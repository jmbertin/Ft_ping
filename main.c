#include "includes/ft_ping.h"

PingStatistics	stats = {0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, NULL, 0, 0, 0, 0, 0, 0, {0}, {0}, {0}, 0};

/**
 * The main entry point of the program. It handles initial command-line argument checks,
 * parses options, resolves the destination IP address, and initiates the ping process.
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Returns 0 on successful completion of the program. Exits with an error message for invalid input or execution failure.
 */
int	main(int argc, char *argv[])
{
	if (argc < 2)
		ft_print_error_message("usage error: Destination address required.", 0);
	if (ft_check_args(argc, argv) > 1)
		ft_print_error_message("usage error: No more than one adress.", 0);

	ft_parse_options(argc, argv);

	stats.ip_addrstr = ft_extract_ip(argc, argv);
	stats.ip_addr = ft_resolve_fqdn(stats.ip_addrstr);
	if (stats.ip_addr == NULL)
		ft_print_error_message("unknow host", 1);
	ft_ping();
	return (0);
}
