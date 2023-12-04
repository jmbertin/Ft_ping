#include "includes/ft_ping.h"

extern PingStatistics	stats;

/**
 * Prepares a message header for sending or receiving ICMP packets. Initializes the buffer, message header,
 * and IO vector for network communication.
 * @param msg Pointer to the message header structure to be prepared.
 * @param addr Pointer to the sockaddr_in structure containing the destination address.
 * @param iov Pointer to the IO vector structure for IO operations.
 * @param buffer Buffer to hold the ICMP message.
 */
static void	prepare_msg(struct msghdr *msg, struct sockaddr_in *addr, struct iovec *iov, char *buffer)
{
	socklen_t	addr_len;

	ft_memset(buffer, 0, 1024);
	addr_len = sizeof(*addr);
	iov->iov_base = buffer;
	iov->iov_len = 1024;
	ft_memset(msg, 0, sizeof(*msg));
	msg->msg_name = addr;
	msg->msg_namelen = addr_len;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
}

/**
 * Receives and processes an ICMP reply. Handles various scenarios like time exceeded, echo reply, and
 * determines if the ping operation should continue or end. Calculates and records response times.
 */
static void	rcv_reply()
{
	char			buffer[1024];
	struct iphdr	*ip_header;
	struct icmphdr	*icmp_header;
	struct msghdr	msg;
	struct iovec	iov;
	int				message_size;

	struct timeval	tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	setsockopt(stats.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	prepare_msg(&msg, &stats.addr, &iov, buffer);
	message_size = recvmsg(stats.sockfd, &msg, 0);
	ip_header = (struct iphdr *)buffer;
	icmp_header = (struct icmphdr *)(buffer + (ip_header->ihl * 4));

	char ipstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(stats.addr.sin_addr), ipstr, sizeof(ipstr));

	if (icmp_header->type == ICMP_DEST_UNREACH)
	{
		printf("From %s icmp_seq=%d Destination Host Unreachable\n", ipstr, stats.sequence);
		if (stats.verbose)
			ft_print_error_infos(ip_header, icmp_header);
	}
	else if (icmp_header->type == ICMP_TIME_EXCEEDED)
	{
		printf("%lu bytes from _gateway (%s): Time to live exceeded\n", message_size - sizeof(struct iphdr), ipstr);
		if (stats.verbose)
			ft_print_error_infos(ip_header, icmp_header);
		if (stats.packets_sent == stats.option_c_value || (ft_compare_time() && stats.option_w))
			ft_sigint_handler(SIGINT);
		close(stats.sockfd);
		alarm(1);
		return ;
	}
	else if (icmp_header->un.echo.id == (getpid() & 0xFFFF))
	{
		double response_time = ft_calc_timediff(&stats.send_time);
		stats.total_time += response_time / 1000;
		if (stats.packets_received == 0)
		{
			stats.min_time = response_time / 1000;
			stats.max_time = response_time / 1000;
		}
		else
		{
			if (response_time / 1000 < stats.min_time)
				stats.min_time = response_time / 1000;
			if (response_time / 1000 > stats.max_time)
				stats.max_time = response_time / 1000;
		}
		if (stats.packets_received < MAX_PACKETS)
			stats.response_times[stats.packets_received] = response_time / 1000;
		if (!stats.flood)
			ft_print(message_size - sizeof(struct iphdr), ip_header->ttl, response_time, icmp_header->un.echo.id);
		else if (stats.flood && stats.packets_received == 500)
			ft_printf("....");
		stats.packets_received++;
	}
	if (stats.packets_sent - 1 == stats.option_c_value && stats.option_c_value)
	{
		stats.packets_sent--;
		ft_sigint_handler(SIGINT);
	}
	if (ft_compare_time() && stats.option_w)
		ft_sigint_handler(SIGINT);
	alarm(1);
}

/**
 * Creates a raw socket for ICMP communication and initializes the address structure for the destination IP.
 * Handles errors in socket creation and address conversion, exiting the program if any occur.
 */
static void	create_socket_and_init_addr()
{
	int	ret;

	stats.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (stats.sockfd < 0)
		ft_print_error_message("socket: socket creation failed", 1);

	stats.addr.sin_family = AF_INET;
	stats.addr.sin_port = 0;
	stats.addr.sin_addr.s_addr = inet_addr(stats.ip_addr);
	ret = inet_pton(AF_INET, stats.ip_addr, &(stats.addr.sin_addr));
	if (ret <= 0)
	{
		if (ret == 0)
			ft_print_error_message("inet_pton: invalid address", 1);
		else
			ft_print_error_message("inet_pton: inet_pton failed", 1);
	}
}

/**
 * Constructs and sends an ICMP echo request packet. Initializes the packet with the ICMP header,
 * sets the necessary fields like type, code, and checksum, and sends it to the destination IP.
 */
static void	send_icmp()
{
	char				packet[64];
	struct icmphdr		*icmph;

	if (stats.option_i)
		usleep(stats.option_i_value * 1000000);

	ft_bzero(&stats.addr, sizeof(struct sockaddr_in));
	create_socket_and_init_addr();

	ft_memset(packet, 0, sizeof(packet));
	icmph = (struct icmphdr *)packet;
	icmph->type = ICMP_ECHO;
	icmph->code = 0;
	icmph->un.echo.id = getpid() & 0xFFFF;
	icmph->un.echo.sequence = stats.sequence;
	icmph->checksum = ft_checksum(packet, sizeof(packet));
	gettimeofday(&stats.send_time, NULL);
	if (sendto(stats.sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&stats.addr, sizeof(stats.addr)) < 0)
		ft_print_error_message("sendto: sendto failed", 1);
	stats.packets_sent++;
	stats.sequence++;
}

/**
 * Main function to perform the ping operation. It prints initial ping information, sets up signal handlers,
 * and enters a loop to continuously send and receive ICMP packets. Handles verbose mode and flood mode
 * for ping requests.
 */
void		ft_ping()
{
	if (!stats.verbose)
		printf("PING %s (%s) 56 data bytes\n", stats.ip_addrstr, stats.ip_addr);
	else
		printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", stats.ip_addrstr, stats.ip_addr, getpid() & 0xFFFF, getpid() & 0xFFFF);
	signal(SIGALRM, send_icmp);
	signal(SIGINT, ft_sigint_handler);
	send_icmp();
	while(1)
	{
		if (stats.flood)
			send_icmp();
		rcv_reply();
	}
}
