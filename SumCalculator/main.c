#include <stdio.h>

int main() {
    float a, b;
    printf("Voer twee getallen in: ");
    scanf("%f %f", &a, &b);

    printf("Som: %f\n", a + b);
    printf("Verschil: %f\n", a - b);
    printf("Product: %f\n", a * b);

    return 0;
}
