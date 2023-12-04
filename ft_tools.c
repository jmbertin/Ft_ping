/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbertin <jbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:41:24 by jbertin           #+#    #+#             */
/*   Updated: 2023/11/30 22:53:51 by jbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_ping.h"

extern PingStatistics	stats;

/**
 * Computes the square root of a given number using a binary search method.
 * Returns 0 for non-positive inputs.
 * @param x The number for which the square root is to be calculated.
 * @return The square root of x.
 */
double			ft_simple_sqrt(double x)
{
	if (x <= 0)
		return 0;

	double	low = 0;
	double	high = x;
	double	mid = (low + high) / 2;

	while (high - low > 0.00001)
	{
		if (mid * mid > x)
			high = mid;
		else
			low = mid;
		mid = (low + high) / 2;
	}

	return (mid);
}

/**
 * Calculates the power of a base number raised to a given exponent.
 * Handles negative exponents by returning the reciprocal of the result for positive exponents.
 * @param base The base number.
 * @param exp The exponent.
 * @return The result of base raised to the power of exp.
 */
double			ft_simple_pow(double base, int exp)
{
	if (exp == 0)
		return (1);

	double	result = 1;
	for (int i = 0; i < exp; i++)
		result *= base;

	if (exp < 0)
		return (1 / result);

	return (result);
}

/**
 * Computes the checksum for a given buffer of data. Used for verifying the integrity of data.
 * @param b Pointer to the buffer containing the data.
 * @param len Length of the buffer in bytes.
 * @return The computed checksum value.
 */
unsigned short	ft_checksum(void *b, int len)
{
	unsigned short	*buf;
	unsigned int	sum;
	unsigned short	result;

	buf = b;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}

/**
 * Calculates the difference in time between the current time and a specified start time.
 * @param send_time Pointer to a timeval structure holding the start time.
 * @return The time difference in microseconds.
 */
float			ft_calc_timediff(struct timeval *send_time)
{
	float			time;
	struct timeval	end;

	gettimeofday(&end, NULL);
	time = ((end.tv_sec - send_time->tv_sec) * 1000000) + (end.tv_usec - send_time->tv_usec);
	return (time);
}

/**
 * Resolves a fully qualified domain name (FQDN) to its corresponding IP address.
 * @param ip_addrstr String containing the FQDN to be resolved.
 * @return Pointer to a string containing the resolved IP address. Returns NULL if resolution fails.
 */
char			*ft_resolve_fqdn(char *ip_addrstr)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	int				errcode;
	static char		addrstr[INET_ADDRSTRLEN];

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	errcode = getaddrinfo(ip_addrstr, NULL, &hints, &res);
	if (errcode != 0)
		return (NULL);
	inet_ntop(res->ai_family, &((struct sockaddr_in *)res->ai_addr)->sin_addr, addrstr, INET_ADDRSTRLEN);
	freeaddrinfo(res);
	if (addrstr[0] == '\0')
		ft_print_error_message("unknow host", 1);
	return (addrstr);
}

/**
 * Compares the elapsed time since a predefined start time against a specified maximum duration.
 * @return 1 if the elapsed time exceeds the maximum duration, 0 otherwise.
 */
int				ft_compare_time()
{
	struct timeval now;
	gettimeofday(&now, NULL);

	long	elapsed_seconds = now.tv_sec - stats.timestarted.tv_sec;
	long	elapsed_useconds = now.tv_usec - stats.timestarted.tv_usec;
	double	elapsed = elapsed_seconds + (elapsed_useconds / 1000000.0);

	if (elapsed > stats.option_w_value)
		return (1);
	else
		return (0);
}
