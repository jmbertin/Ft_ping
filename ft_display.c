/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbertin <jbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:40:39 by jbertin           #+#    #+#             */
/*   Updated: 2023/11/30 23:36:28 by jbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_ping.h"

extern PingStatistics	stats;

/**
 * Prints the details of the ICMP echo reply, including message size, IP address, sequence number,
 * identifier, TTL, and response time. Formats output based on verbose setting in `stats`.
 * @param message_size The size of the ICMP message in bytes.
 * @param ttl Time-to-live value of the ICMP packet.
 * @param time Round trip time for the ICMP packet in microseconds.
 * @param id Identifier of the ICMP packet.
 */
void	ft_print(unsigned long int message_size, int ttl, double time, int id)
{
	char	ipstr[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(stats.addr.sin_addr), ipstr, sizeof(ipstr));
	if (stats.verbose)
		printf("%lu bytes from %s: icmp_seq=%d ident=%d ttl=%d time=%.3f ms\n", message_size, ipstr, stats.sequence, id, ttl, time / 1000);
	else
		printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", message_size, ipstr, stats.sequence, ttl, time / 1000);
}

/**
 * Prints the help message for the `ping` command, listing all available options and their descriptions.
 * Exits the program after displaying the help message.
 * @param exit_code The exit code with which the program will terminate.
 */
void	ft_print_help(int exit_code)
{
	printf("\nUsage: ping [OPTION...] HOST...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf(" Options valid for all request types:\n");
	printf("  -h                 give this help list\n");
	printf("  -c <NUMBER>        stop after sending NUMBER packets\n");
	printf("  -i <NUMBER>        wait NUMBER seconds between sending each packet\n");
	printf("  -w <NUMBER>        stop after NUMBER seconds\n");
	printf("  -f                 flood ping\n");
	printf("  -v                 verbose output\n");
	exit(exit_code);
}

/**
 * Prints a specified error message and exits the program. Optionally frees the global IP address string.
 * @param message The error message to be displayed.
 * @param must_free Flag indicating whether to free the global IP address string (non-zero) or not (zero).
 */
void	ft_print_error_message(const char *message, int must_free)
{
	fprintf(stderr, "ft_ping: %s\n", message);
	if (must_free)
		free(stats.ip_addrstr);
	exit(1);
}

/**
 * Prints detailed error information, including a dump of the IP header and a summary of the ICMP header.
 * Extracts and displays source and destination IP addresses along with other header details.
 * @param ip_header Pointer to the IP header structure.
 * @param icmp_header Pointer to the ICMP header structure.
 */
void	ft_print_error_infos(struct iphdr *ip_header, struct icmphdr *icmp_header)
{
	char	src_ip[INET_ADDRSTRLEN];
	char	dst_ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(ip_header->saddr), src_ip, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(ip_header->daddr), dst_ip, INET_ADDRSTRLEN);

	printf("IP Hdr Dump:\n");
	long unsigned int	i = -1;
	while (++i < sizeof(*ip_header))
		printf("%s%02x", ((i % 2) ? "" : " "), *((unsigned char *)ip_header + i));
	printf("\nVr HL  TOS  Len   ID Flg  off TTL Pro  cks             Src             Dst Data\n");
	printf(" %x  %x %03x %04x %04x %03x %04x %03x %03x %04x %15s %15s\n",
		ip_header->version, ip_header->ihl, ntohs(ip_header->tos), ntohs(ip_header->tot_len),
		ntohs(ip_header->id), ntohs(ip_header->frag_off) >> 13, ntohs(ip_header->frag_off) & 0x1FFF,
		ip_header->ttl, ip_header->protocol, ntohs(ip_header->check), dst_ip, stats.ip_addr);

	printf("ICMP: type 8, code %d, size 64, id 0x%04x, seq 0x%04x\n",
		icmp_header->code, getpid() & 0xFFFF, stats.sequence - 1);
}

/**
 * Signal handler for SIGINT (Ctrl+C). Prints ping statistics including packet loss and round-trip time
 * statistics (min, avg, max, stddev), then exits the program. Statistics are based on packets sent and received.
 * @param signal The signal number (expected to be SIGINT).
 */
void	ft_sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n--- %s ping statistics ---\n", stats.ip_addrstr);
		double	packet_loss = 100.0 * (1 - (double)stats.packets_received / stats.packets_sent);
		double	avg_time = stats.packets_received ? stats.total_time / stats.packets_received : 0;
		double	sum_sq_diff = 0.0;
		for (int i = 0; i < stats.packets_received; i++)
			sum_sq_diff += ft_simple_pow(stats.response_times[i] - avg_time, 2);
		double	stddev = ft_simple_sqrt(sum_sq_diff / stats.packets_received);
		if (stats.flood && stats.packets_sent - stats.packets_received == 2)
			packet_loss = 0;
		printf("%d packets transmitted, %d packets received, %.2f%% packet loss\n", stats.packets_sent, stats.packets_received, packet_loss);
		if (stats.packets_received > 0)
			printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", stats.min_time, avg_time, stats.max_time, stddev);
		free(stats.ip_addrstr);
		exit(0);
	}
}
