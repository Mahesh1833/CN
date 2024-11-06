def decimal_to_binary(decimal):
    return f"{decimal:08b}"

def binary_to_decimal(binary):
    return int(binary, 2)

def ip_to_binary(ip):
    binary_ip = ""
    for octet in ip.split('.'):
        binary_ip += decimal_to_binary(int(octet))
    return binary_ip

def binary_to_ip(binary):
    ip = []
    for i in range(0, 32, 8):
        ip.append(str(binary_to_decimal(binary[i:i+8])))
    return '.'.join(ip)

def subnet_mask(subnet_bits):
    mask_binary = '1' * subnet_bits + '0' * (32 - subnet_bits)
    return binary_to_ip(mask_binary)

def network_address(ip_binary, subnet_bits):
    network_binary = ip_binary[:subnet_bits] + '0' * (32 - subnet_bits)
    return binary_to_ip(network_binary)

def broadcast_address(ip_binary, subnet_bits):
    broadcast_binary = ip_binary[:subnet_bits] + '1' * (32 - subnet_bits)
    return binary_to_ip(broadcast_binary)

def host_range(network, broadcast):
    first_host = network.split('.')
    last_host = broadcast.split('.')
    first_host[-1] = str(int(first_host[-1]) + 1)
    last_host[-1] = str(int(last_host[-1]) - 1)
    return '.'.join(first_host), '.'.join(last_host)

def subnetting(ip, subnet_bits):
    ip_binary = ip_to_binary(ip)
    subnet_mask_value = subnet_mask(subnet_bits)
    network = network_address(ip_binary, subnet_bits)
    broadcast = broadcast_address(ip_binary, subnet_bits)
    first_host, last_host = host_range(network, broadcast)
    num_hosts = (2 ** (32 - subnet_bits)) - 2

    print(f"IP Address: {ip}")
    print(f"Subnet Mask: {subnet_mask_value}")
    print(f"Network Address: {network}")
    print(f"Broadcast Address: {broadcast}")
    print(f"Total Number of Hosts: {num_hosts}")
    print(f"Host Range: {first_host} - {last_host}")

subnetting("192.168.1.1", 24)



