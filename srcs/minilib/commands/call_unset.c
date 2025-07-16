#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Count the number of environment variables
int env_count(char **envp) {
    int count = 0;
    while (envp && envp[count])
        count++;
    return count;
}

// Find index of a variable in envp by name (before '=')
int find_var_index(char **envp, const char *name) {
    size_t len = strlen(name);
    for (int i = 0; envp[i]; i++) {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return i;
    }
    return -1;
}

// Unset (remove) a variable from envp
void unset_var(char ***envp_ref, const char *name) {
    char **envp = *envp_ref;
    int index = find_var_index(envp, name);
    if (index == -1)
        return; // Variable not found

    int count = env_count(envp);
    char **new_env = malloc(sizeof(char *) * count); // one less slot
    if (!new_env)
        return;

    int j = 0;
    for (int i = 0; i < count; i++) {
        if (i == index) {
            free(envp[i]); // Free the removed variable
            continue;
        }
        new_env[j++] = envp[i];
    }
    new_env[j] = NULL;

    free(envp); // Free old array (not the strings, already transferred)
    *envp_ref = new_env;
}
int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;

    // Step 1: Copy envp to a dynamic editable array
    int count = env_count(envp);
    char **my_env = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++)
        my_env[i] = strdup(envp[i]);
    my_env[count] = NULL;

    // Step 2: Remove a variable
    unset_var(&my_env, "OLDPWD");

    // Step 3: Print updated env
    printf("Updated env:\n");
    for (int i = 0; my_env[i]; i++)
        printf("%s\n", my_env[i]);

    // Step 4: Free
    for (int i = 0; my_env[i]; i++)
        free(my_env[i]);
    free(my_env);

    return 0;
}