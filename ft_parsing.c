/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbertin <jbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:41:24 by jbertin           #+#    #+#             */
/*   Updated: 2023/11/30 22:53:10 by jbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_ping.h"

extern PingStatistics	stats;

/// A VERIFIER
/**
 * Extracts the IP address from the command-line arguments. It skips any options (strings starting with '-')
 * and returns the first non-option argument as the IP address.
 * @param argv Array of command-line arguments.
 * @param argc The number of command-line arguments.
 * @return Pointer to the extracted IP address string. If no valid IP address is found, an error message is printed and the program exits.
 */
char		*ft_extract_ip(int argc, char **argv)
{
	int		i;
	char	*ip_addr;

	i = 1;
	ip_addr = NULL;
	while (i < argc)
	{
		if (argv[i][0] != '-' && (ft_strncmp(argv[i - 1], "-c", 2) != 0 && ft_strncmp(argv[i - 1], "-i", 2) != 0 && ft_strncmp(argv[i - 1], "-w", 2) != 0))
		{
			ip_addr = ft_strdup(argv[i]);
			break ;
		}
		i++;
	}
	if (!ip_addr)
		ft_print_error_message("usage error: Destination address required.", 0);
	return (ip_addr);
}

/**
 * Counts the number of destination addresses (non-option arguments) provided in the command-line arguments.
 * Skips over options and their respective values.
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return The count of non-option arguments (destination addresses).
 */
int			ft_check_args(int argc, char **argv)
{
	int	i;
	int	count_dest;

	count_dest = 0;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-' && argv[i - 1][0] != '-')
			count_dest++;
		else if (argv[i][0] != '-' && (ft_strncmp(argv[i - 1], "-c", 2) != 0 && ft_strncmp(argv[i - 1], "-i", 2) != 0 && ft_strncmp(argv[i - 1], "-w", 2) != 0))
			count_dest++;
		i++;
	}
	return (count_dest);
}

/**
 * Prints an error message based on the provided error code and the name of the option causing the error.
 * Calls `ft_print_help` and then exits the program.
 * @param option_name The name of the option that triggered the error.
 * @param error_code The specific error code indicating the type of error.
 */
static void	error_invalid_option(const char *option_name, int error_code)
{
	if (error_code == 1)
		fprintf(stderr, "ft_ping: invalid option -- '%s'\n", option_name);
	else if (error_code == 2)
		fprintf(stderr, "ft_ping: usage error: %s option requires a positive integer argument.\n", option_name);
	else if (error_code == 3)
		fprintf(stderr, "ft_ping: usage error: -f and -i options are incompatible.\n");
	ft_print_help(1);
}

/**
 * Processes an option that requires an integer value. Sets the respective flag and value in the global stats structure.
 * Validates the argument to ensure it is a positive integer. If validation fails, calls `error_invalid_option`.
 * @param arg The argument string to be processed as an integer.
 * @param option_flag Pointer to the flag in the global stats structure indicating whether the option is set.
 * @param option_value Pointer to the value in the global stats structure to store the integer value.
 * @param option_name The name of the option being processed.
 */
static void	process_integer_option(char *arg, int *option_flag, int *option_value, const char *option_name)
{
	*option_flag = 1;
	*option_value = atoi(arg);
	if (ft_strncmp(option_name, "w", 2) == 0)
		gettimeofday(&stats.timestarted, NULL);
	if (*option_value <= 0)
		error_invalid_option(option_name, 2);
}

/**
 * Parses the command-line arguments to set various options in the global stats structure.
 * Handles options like '-h', '-v', '-f', '-c', '-i', '-w' and validates them.
 * In case of invalid options or incompatible combinations, an error message is printed and the program exits.
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line arguments.
 */
void		ft_parse_options(int argc, char **argv)
{
	int	i = 1;

	while (i < argc)
	{
		if (argv[i][0] == '-')
		{
			if (strlen(argv[i]) != 2)
				error_invalid_option(argv[i], 1);
			switch (argv[i][1])
			{
				case 'h':
					if (strlen(argv[i]) == 2) ft_print_help(0);
					break;
				case 'v':
					if (strlen(argv[i]) == 2) stats.verbose = 1;
					break;
				case 'f':
					if (strlen(argv[i]) == 2) stats.flood = 1;
					break;
				case 'c':
					if (strlen(argv[i]) == 2 && i + 1 < argc) process_integer_option(argv[++i], &stats.option_c, &stats.option_c_value, "c");
					else error_invalid_option("c", 2);
					break;
				case 'i':
					if (strlen(argv[i]) == 2 && i + 1 < argc) process_integer_option(argv[++i], &stats.option_i, &stats.option_i_value, "i");
					else error_invalid_option("i", 2);
					break;
				case 'w':
					if (strlen(argv[i]) == 2 && i + 1 < argc) process_integer_option(argv[++i], &stats.option_w, &stats.option_w_value, "w");
					else error_invalid_option("w", 2);
					break;
				default:
					error_invalid_option(argv[i], 1);
			}
		}
		i++;
	}

	if (stats.flood && stats.option_i)
		error_invalid_option("", 3);
}
