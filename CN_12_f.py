import socket

def dns_lookup(query):
    try:
        # Check if the input is an IP address
        socket.inet_aton(query)
        # Perform reverse DNS lookup (IP to URL)
        result = socket.gethostbyaddr(query)
        print(f"IP Address: {query}\nURL: {result[0]}")
    except socket.error:
        try:
            # Perform forward DNS lookup (URL to IP)
            result = socket.gethostbyname(query)
            print(f"URL: {query}\nIP Address: {result}")
        except socket.error:
            print(f"Invalid input: {query}")

# Test the function with an IP or URL input
if __name__ == "__main__":
    user_input = input("Enter a URL or IP address: ")
    dns_lookup(user_input)
