namespace TextEditor
{
    class Program
    {
        static List<string> lines = new List<string> { "" };
        static int cursorLine = 0;
        static int cursorPos = 0;

        static Stack<List<string>> undoStack = new Stack<List<string>>();
        static Stack<List<string>> redoStack = new Stack<List<string>>();

        static void Main(string[] args)
        {
            ConsoleKeyInfo key;
            Console.Clear();
            DrawEditor();

            while (true)
            {
                key = Console.ReadKey(true);

                if (key.Key == ConsoleKey.Escape)
                    break;

                if (key.Modifiers == ConsoleModifiers.Control && key.Key == ConsoleKey.U)
                {
                    Undo();
                }
                else if (key.Modifiers == ConsoleModifiers.Control && key.Key == ConsoleKey.R)
                {
                    Redo();
                }
                else
                {
                    SaveUndoState();

                    if (key.Key == ConsoleKey.UpArrow)
                    {
                        if (cursorLine > 0)
                        {
                            cursorLine--;
                            cursorPos = Math.Min(cursorPos, lines[cursorLine].Length);
                        }
                    }
                    else if (key.Key == ConsoleKey.DownArrow)
                    {
                        if (cursorLine < lines.Count - 1)
                        {
                            cursorLine++;
                            cursorPos = Math.Min(cursorPos, lines[cursorLine].Length);
                        }
                    }
                    else if (key.Key == ConsoleKey.LeftArrow)
                    {
                        if (cursorPos > 0)
                        {
                            cursorPos--;
                        }
                        else if (cursorLine > 0)
                        {
                            cursorLine--;
                            cursorPos = lines[cursorLine].Length;
                        }
                    }
                    else if (key.Key == ConsoleKey.RightArrow)
                    {
                        if (cursorPos < lines[cursorLine].Length)
                        {
                            cursorPos++;
                        }
                        else if (cursorLine < lines.Count - 1)
                        {
                            cursorLine++;
                            cursorPos = 0;
                        }
                    }
                    else if (key.Key == ConsoleKey.Enter)
                    {
                        string currentLine = lines[cursorLine];
                        string newLine = "";
                        if (cursorPos < currentLine.Length)
                        {
                            newLine = currentLine.Substring(cursorPos);
                            lines[cursorLine] = currentLine.Substring(0, cursorPos);
                        }
                        else
                        {
                            lines[cursorLine] = currentLine;
                        }
                        lines.Insert(cursorLine + 1, newLine);
                        cursorLine++;
                        cursorPos = 0;
                    }
                    else if (key.Key == ConsoleKey.Backspace)
                    {
                        if (cursorPos > 0)
                        {
                            lines[cursorLine] = lines[cursorLine].Remove(cursorPos - 1, 1);
                            cursorPos--;
                        }
                        else if (cursorLine > 0)
                        {
                            cursorPos = lines[cursorLine - 1].Length;
                            lines[cursorLine - 1] += lines[cursorLine];
                            lines.RemoveAt(cursorLine);
                            cursorLine--;
                        }
                    }
                    else if (!char.IsControl(key.KeyChar))
                    {
                        if (cursorPos <= lines[cursorLine].Length)
                        {
                            lines[cursorLine] = lines[cursorLine].Insert(cursorPos, key.KeyChar.ToString());
                            cursorPos++;
                        }
                    }

                    DrawEditor();
                }
            }
        }

        static void SaveUndoState()
        {
            undoStack.Push(new List<string>(lines));
            redoStack.Clear();
        }

        static void Undo()
        {
            if (undoStack.Count > 0)
            {
                redoStack.Push(new List<string>(lines));
                lines = new List<string>(undoStack.Pop());
                cursorLine = Math.Min(cursorLine, lines.Count - 1);
                cursorPos = Math.Min(cursorPos, lines[cursorLine].Length);
                DrawEditor();
            }
        }

        static void Redo()
        {
            if (redoStack.Count > 0)
            {
                undoStack.Push(new List<string>(lines));
                lines = new List<string>(redoStack.Pop());
                cursorLine = Math.Min(cursorLine, lines.Count - 1);
                cursorPos = Math.Min(cursorPos, lines[cursorLine].Length);
                DrawEditor();
            }
        }

        static void DrawEditor()
        {
            Console.Clear();

            int maxLineLength = cursorPos + 1;
            foreach (var line in lines)
            {
                if (line.Length > maxLineLength)
                    maxLineLength = line.Length;
            }

            for (int i = 0; i < lines.Count; i++)
            {
                string paddedLine = PadLine(lines[i], maxLineLength);
                Console.WriteLine(paddedLine);
            }

            int safeLine = Math.Min(cursorLine, lines.Count - 1);
            int safePos = Math.Min(cursorPos, lines[safeLine].Length);
            Console.SetCursorPosition(safePos, safeLine);

            Console.WriteLine("\n\nDruk op ESC om te stoppen. CTRL+U voor Undo, CTRL+R voor Redo.");
        }

        static string PadLine(string line, int length)
        {
            if (line.Length < length)
                return line + new string(' ', length - line.Length);
            else
                return line;
        }
    }
}
