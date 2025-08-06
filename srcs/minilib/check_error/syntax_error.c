#include "../../../include/minishell.h"

int is_invalid_token(t_token_type type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT || 
			type == REDIR_APPEND || type == HERE_DOC);
}

int	is_valid_syntax(t_token *tokens)
{
	t_token *curr = tokens;

	if (!curr)
		return (1);

	// Start with pipe: syntax error
	if (curr->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}

	while (curr)
	{
		if (curr->type == PIPE)
		{
			// After a pipe, we can have redirections or words
			if (!curr->next)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (0);
			}
			// Pipe followed by another pipe is invalid
			if (curr->next->type == PIPE)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
				return (0);
			}
		}
		else if (is_invalid_token(curr->type) && curr->type != PIPE)
		{
			// For redirections, they must be followed by a word
			if (!curr->next || curr->next->type != WORD)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				if (!curr->next)
					ft_putstr_fd("newline", 2);
				else
				{
					if (curr->next->type == PIPE)
						ft_putstr_fd("|", 2);
					else if (curr->next->type == REDIR_IN)
						ft_putstr_fd("<", 2);
					else if (curr->next->type == REDIR_OUT)
						ft_putstr_fd(">", 2);
					else if (curr->next->type == REDIR_APPEND)
						ft_putstr_fd(">>", 2);
					else if (curr->next->type == HERE_DOC)
						ft_putstr_fd("<<", 2);
					else
						ft_putstr_fd(curr->next->value, 2);
				}
				ft_putstr_fd("'\n", 2);
				return (0);
			}
		}
		curr = curr->next;
	}

	// Ends with pipe: syntax error
	curr = tokens;
	while (curr && curr->next)
		curr = curr->next;
	if (curr && curr->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}

	return (1);
}
