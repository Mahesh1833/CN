import sys

class NetworkRouter:
    def __init__(self, vertices):
        self.vertices = vertices
        self.edges = []

    def add_link(self, u, v, weight):
        self.edges.append((u, v, weight))
        self.edges.append((v, u, weight))

    def bellman_ford(self, src):
        dist = [sys.maxsize] * self.vertices
        dist[src] = 0

        for _ in range(self.vertices - 1):
            for u, v, weight in self.edges:
                if dist[u] != sys.maxsize and dist[u] + weight < dist[v]:
                    dist[v] = dist[u] + weight

        for u, v, weight in self.edges:
            if dist[u] != sys.maxsize and dist[u] + weight < dist[v]:
                print("Graph contains a negative-weight cycle")
                return

        self.print_solution(dist, src)

    def print_solution(self, dist, src):
        print(f"Router {src} to other Routers with minimum costs:")
        for i in range(self.vertices):
            print(f"Router {src} to Router {i} : {dist[i]}")

network = NetworkRouter(5)

network.add_link(0, 1, 2)
network.add_link(0, 2, 4)
network.add_link(1, 2, 1)
network.add_link(1, 3, 7)
network.add_link(2, 3, 3)
network.add_link(3, 4, 1)

network.bellman_ford(0)
