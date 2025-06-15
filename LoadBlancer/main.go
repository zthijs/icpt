package main

import (
	"encoding/json"
	"fmt"
	"log"
	"math/rand"
	"net/http"
	"os"
	"strconv"
	"sync"
	"time"
)

type Request struct {
	Weight float64
	RespCh chan string
}

type Server struct {
	ID        int
	TaskQueue chan Request
	Active    int
	Weight    int
}

type LoadBalancer struct {
	Servers    []*Server
	Algo       string
	mutex      sync.Mutex
	rrIndex    int
	wrrIndex   int
	wrrCounter int
}

func main() {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))

	numClients := 5
	numServers := 3

	algo := "round"
	//algo := "weighted"
	//algo := "least"

	if len(os.Args) > 1 {
		algo = os.Args[1]
	}

	lb := NewLoadBalancer(numServers, algo)

	go lb.Start()

	for i := 0; i < numClients; i++ {
		go client(i, lb, r)
	}

	select {}
}

func NewLoadBalancer(numServers int, algo string) *LoadBalancer {
	lb := &LoadBalancer{
		Servers: make([]*Server, numServers),
		Algo:    algo,
	}

	for i := 0; i < numServers; i++ {
		server := &Server{
			ID:        i,
			TaskQueue: make(chan Request, 100),
			Weight:    i%3 + 1,
		}
		lb.Servers[i] = server
		go server.Start()
	}

	return lb
}

func (s *Server) Start() {
	for req := range s.TaskQueue {
		s.Active++
		time.Sleep(time.Duration(req.Weight * float64(time.Second)))
		req.RespCh <- fmt.Sprintf("Server %d handled request (%.2fs)", s.ID, req.Weight)
		s.Active--
	}
}

func (lb *LoadBalancer) Start() {
	http.HandleFunc("/request", lb.HandleRequest)
	log.Println("Load balancer started at :8080 using", lb.Algo, "algorithm")
	log.Fatal(http.ListenAndServe(":8080", nil))
}

func (lb *LoadBalancer) HandleRequest(w http.ResponseWriter, r *http.Request) {
	weightStr := r.URL.Query().Get("weight")
	weight, err := strconv.ParseFloat(weightStr, 64)
	if err != nil || weight <= 0 || weight > 1 {
		http.Error(w, "Invalid weight", http.StatusBadRequest)
		return
	}

	respCh := make(chan string)
	req := Request{Weight: weight, RespCh: respCh}
	lb.Dispatch(req)

	resp := <-respCh
	json.NewEncoder(w).Encode(map[string]string{"response": resp})
}

func (lb *LoadBalancer) Dispatch(req Request) {
	lb.mutex.Lock()
	defer lb.mutex.Unlock()

	switch lb.Algo {
	case "round":
		server := lb.Servers[lb.rrIndex]
		lb.rrIndex = (lb.rrIndex + 1) % len(lb.Servers)
		server.TaskQueue <- req

	case "weighted":
		for {
			server := lb.Servers[lb.wrrIndex]
			if lb.wrrCounter < server.Weight {
				lb.wrrCounter++
				server.TaskQueue <- req
				return
			} else {
				lb.wrrCounter = 1
				lb.wrrIndex = (lb.wrrIndex + 1) % len(lb.Servers)
			}
		}

	case "least":
		min := lb.Servers[0]
		for _, s := range lb.Servers {
			if s.Active < min.Active {
				min = s
			}
		}
		min.TaskQueue <- req
	}
}

func client(id int, lb *LoadBalancer, r *rand.Rand) {
	for {
		weight := r.Float64()
		if weight == 0 {
			weight = 0.1
		}

		start := time.Now()
		resp, err := http.Get(fmt.Sprintf("http://localhost:8080/request?weight=%.2f", weight))
		if err != nil {
			log.Printf("[Client %d] error: %v\n", id, err)
			continue
		}

		var result map[string]string
		json.NewDecoder(resp.Body).Decode(&result)
		resp.Body.Close()

		duration := time.Since(start).Seconds()
		performance := duration - weight
		log.Printf("[Client %d] Req: %.2fs | Total: %.2fs | Perf: %.2fs | %s\n",
			id, weight, duration, performance, result["response"])

		time.Sleep(time.Millisecond * time.Duration(500+r.Intn(500)))
	}
}
