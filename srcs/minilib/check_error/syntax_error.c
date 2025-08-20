/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      ::+:    :+:   */
/*                                                                            */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_invalid_token(t_token_type type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HERE_DOC);
}

static void	print_token_error(t_token *token)
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

static int	validate_redirection(t_token *curr)
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

static int	validate_pipe(t_token *curr)
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

static int	validate_redirection_sequence(t_token *tokens)
{
	t_token	*curr;
	int		redir_count;

	curr = tokens;
	redir_count = 0;
	while (curr)
	{
		if (curr->type == WORD)
		{
			redir_count = 0;
		}
		else if (curr->type == PIPE)
		{
			redir_count = 0;
		}
		else if (is_invalid_token(curr->type) && curr->type != PIPE)
		{
			redir_count++;
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
			// Check for consecutive redirections (like < 2 > 3)
			// This should only trigger if we have REDIR_IN WORD REDIR_IN (no space between redirections)
			if (curr->next && curr->next->next && 
				curr->next->next->type == curr->type)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				ft_putstr_fd(curr->next->value, 2);
				ft_putstr_fd("'\n", 2);
				return (0);
			}
		}
		curr = curr->next;
	}
	// Allow standalone redirections (no command required)
	return (1);
}

int	is_valid_syntax(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	if (!curr)
		return (1);
	if (curr->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	while (curr)
	{
		if (curr->type == PIPE)
		{
			if (!validate_pipe(curr))
				return (0);
		}
		else if (is_invalid_token(curr->type) && curr->type != PIPE)
		{
			if (!validate_redirection(curr))
				return (0);
		}
		curr = curr->next;
	}
	return (validate_redirection_sequence(tokens));
}
