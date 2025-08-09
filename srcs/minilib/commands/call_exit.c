/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/08 08:38:11 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int is_numeric(const char *str)
{
    int i = 0;

    if (!str || str[0] == '\0')
        return 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (!str[i]) // '+' or '-' only is invalid
        return 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

static int is_valid_exit_arg(const char *str)
{
    if (!str || str[0] == '\0')
        return 0;

    int sign_count = 0;
    int i = 0;

    while (str[i] == '+' || str[i] == '-')
    {
        sign_count++;
        i++;
    }
    if (sign_count > 1)
        return 0;
    if (!str[i]) // only signs without digits
        return 0;

    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

void call_exit(t_minishell *mini, char **argv)
{
    long long code;

    // Only print "exit" if we're in the main shell, not in a child process (pipe)
    if (!is_in_child_process())
        ft_putstr_fd("exit\n", STDERR_FILENO);

    if (!argv[1])
        exit(mini->exit_status);

    if (!is_valid_exit_arg(argv[1]))
    {
        // Only print error messages if we're in the main shell
        if (!is_in_child_process())
        {
            ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
            ft_putstr_fd(argv[1], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        }
        exit(2);  // bash exits immediately with status 2 on invalid numeric arg
    }

    if (argv[2])
    {
        // Only print error messages if we're in the main shell
        if (!is_in_child_process())
            ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        
        // In child process, just exit with error status
        if (is_in_child_process())
            exit(1);
        
        mini->exit_status = 1;
        return;
    }

    code = ft_atoi(argv[1]);

    // mimic bash behavior: exit code = code modulo 256
    exit((unsigned char)code);
}
