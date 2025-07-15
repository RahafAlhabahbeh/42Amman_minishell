t_token **split_into_commands(t_token *tokens, int *num_cmds)
{
	int count = 1;
	t_token *tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOK_PIPE)
			count++;
		tmp = tmp->next;
	}
	*num_cmds = count;

	t_token **cmds = malloc(sizeof(t_token *) * count);
	int i = 0;
	t_token *start = tokens;
	tmp = tokens;

	while (tmp)
	{
		if (tmp->type == TOK_PIPE)
		{
			tmp->type = TOK_EOF; // Mark split point
			cmds[i++] = start;
			start = tmp->next;
		}
		tmp = tmp->next;
	}
	cmds[i] = start;
	return cmds;
}
char **token_list_to_argv(t_token *head)
{
	int count = 0;
	t_token *tmp = head;
	while (tmp && tmp->type != TOK_PIPE && tmp->type != TOK_EOF)
	{
		if (tmp->type == TOK_WORD)
			count++;
		tmp = tmp->next;
	}

	char **argv = malloc(sizeof(char *) * (count + 1));
	tmp = head;
	int i = 0;
	while (tmp && tmp->type != TOK_PIPE && tmp->type != TOK_EOF)
	{
		if (tmp->type == TOK_WORD)
			argv[i++] = strdup(tmp->value);
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return argv;
}
void execute_pipeline(t_token *tokens, char **envp)
{
	int num_cmds;
	t_token **cmds = split_into_commands(tokens, &num_cmds);

	int pipes[num_cmds - 1][2];
	for (int i = 0; i < num_cmds - 1; i++)
		if (pipe(pipes[i]) == -1)
			perror("pipe");

	for (int i = 0; i < num_cmds; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < num_cmds - 1)
				dup2(pipes[i][1], STDOUT_FILENO);

			for (int j = 0; j < num_cmds - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			char **argv = token_list_to_argv(cmds[i]);
			execvp(argv[0], argv);
			perror("execvp");
			exit(1);
		}
	}

	for (int i = 0; i < num_cmds - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (int i = 0; i < num_cmds; i++)
		wait(NULL);

	free(cmds); 
}
void start_execution(t_token *token_head, char **envp)
{
	execute_pipeline(token_head, envp);
}

