/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      ::+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 12:34:29 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_invalid_token(t_token_type type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HERE_DOC);
}

void	print_token_error(t_token *token)
{
	if (token->type == PIPE)
		ft_putstr_fd("|", 2);
	else if (token->type == REDIR_IN)
		ft_putstr_fd("<", 2);
	else if (token->type == REDIR_OUT)
		ft_putstr_fd(">", 2);
	else if (token->type == REDIR_APPEND)
		ft_putstr_fd(">>", 2);
	else if (token->type == HERE_DOC)
		ft_putstr_fd("<<", 2);
	else
		ft_putstr_fd(token->value, 2);
}

int	validate_redirection(t_token *curr)
{
	if (!curr->next || curr->next->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (!curr->next)
			ft_putstr_fd("newline", 2);
		else
			print_token_error(curr->next);
		ft_putstr_fd("'\n", 2);
		return (0);
	}
	return (1);
}

int	validate_pipe(t_token *curr)
{
	if (!curr->next)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("`newline'\n", 2);
		return (0);
	}
	if (curr->next->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	return (1);
}
