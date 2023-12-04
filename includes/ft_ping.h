#ifndef FT_PING_H
# define FT_PING_H

# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <errno.h>
# include "libft.h"
# include <signal.h>
# include <math.h>

#define MAX_PACKETS 1000
typedef struct
{
	int					verbose;
	int					flood;
	int					option_c;
	int					option_c_value;
	int					option_i;
	int					option_i_value;
	int					option_w;
	int					option_w_value;
	struct timeval		timestarted;
	char				*ip_addr;
	char				*ip_addrstr;
	int					sockfd;
	int					packets_sent;
	int					packets_received;
	double				min_time;
	double				max_time;
	double				total_time;
	double				response_times[MAX_PACKETS];
	struct timeval		send_time;
	struct sockaddr_in	addr;
	int					sequence;
} PingStatistics;

// FT_TOOLS
double			ft_simple_sqrt(double x);
double			ft_simple_pow(double base, int exp);
unsigned short	ft_checksum(void *b, int len);
float			ft_calc_timediff(struct timeval *send_time);
char			*ft_resolve_fqdn(char *ip_addrstr);
int				ft_compare_time();

// FT_PARSING
char			*ft_extract_ip(int argc, char **argv);
int				ft_check_args(int argc, char **argv);
void			ft_parse_options(int argc, char **argv);

// FT_PING
void			ft_ping();

// FT_DISPLAY
void			ft_print_error_infos(struct iphdr *ip_header, struct icmphdr *icmp_header);
void			ft_print_help(int exit_code);
void			ft_print(unsigned long int message_size, int ttl, double time, int id);
void			ft_sigint_handler(int signal);
void			ft_print_error_message(const char *message, int must_free);

#endif
