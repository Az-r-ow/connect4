# **CLI Connect 4**

## V3

### Monte Carlo tree Search

**Monte Carlo tree search** (MCTS) is a heuristic search algorithm for some kinds of decision processes. Hence, perfect for board games such as connect 4.
To balance between exploration and exploitation, the UCT formula will be used since it's the formula that combines the node value concept of MCTS and the UCB formula. ([Read More](https://www.cs.swarthmore.edu/~mitchell/classes/cs63/f20/reading/mcts.html))

The first issue I face facing was that the algorithm sometimes overlooks the possibility of a terminal move from the opponent. Instead it choses a move that might seem better to him (when it's clearly not the case). However this was due to the **Exploration** constant **C** being set too high at 2. After decreasing it to 1.4 (almost root(2) which is the recommended exploration for the UCT function).

## V2

### **Quickstart:**

After cloning create the following folders :

```
mkdir bin obj
```

Then run the following command :

```
make all
```

Finally launch the game with :

```
./bin/connect4v2
```

## V1

### **Quickstart :**

1. Clone the repo

2. Navigate into the repo

3. Compile the c file : **Eg:** `make connect4` or `gcc connect4.c -o connect4`

4. Run `./connect4` (if `make connect4` was used).

> PS: Everything should be done within the terminal

## **Roadmap :**

- Build an interface with NCurses
- Introduce playing against an AI

---

## **Contribute :**

Pull requests are welcome! If you see something you'd like to add, please do. For drastic changes, please open an issue first.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
