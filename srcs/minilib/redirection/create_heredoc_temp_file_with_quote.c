/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc_temp_file_with_quote.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 15:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	init_heredoc_context(t_heredoc_context *ctx,
		const char *delimiter, char **temp_filename_ptr, char quote_char)
{
	*temp_filename_ptr = generate_heredoc_filename(ctx->counter++);
	if (!*temp_filename_ptr)
		return (-1);
	ctx->fd = open(*temp_filename_ptr, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (ctx->fd < 0)
		return (-1);
	ctx->expand_vars = (quote_char != '\'' && quote_char != '"');
	ctx->clean_delim = ft_strdup(delimiter);
	if (!ctx->clean_delim)
		return (-1);
	return (0);
}

int	create_heredoc_temp_file_with_quote(t_minishell *mini,
		const char *delimiter, char **temp_filename_ptr, char quote_char)
{
	t_heredoc_context	ctx;
	struct sigaction	sa;
	struct sigaction	old_sa;

	ctx.counter = 0;
	if (init_heredoc_context(&ctx, delimiter,
			temp_filename_ptr, quote_char) < 0)
		return (-1);
	setup_heredoc_signal(&sa, &old_sa);
	if (write_heredoc_lines(mini, ctx.fd, ctx.clean_delim, ctx.expand_vars) < 0)
	{
		close(ctx.fd);
		unlink(*temp_filename_ptr);
		free(ctx.clean_delim);
		sigaction(SIGINT, &old_sa, NULL);
		return (-1);
	}
	close(ctx.fd);
	free(ctx.clean_delim);
	sigaction(SIGINT, &old_sa, NULL);
	return (open(*temp_filename_ptr, O_RDONLY));
}
