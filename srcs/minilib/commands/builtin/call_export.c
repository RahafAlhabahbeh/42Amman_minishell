#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Count current environment size
int env_count(char **envp) {
    int count = 0;
    while (envp && envp[count])
        count++;
    return count;
}

// Find index of variable by name
int find_var_index(char **envp, const char *name) {
    size_t len = strlen(name);
    for (int i = 0; envp[i]; i++) {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return i;
    }
    return -1;
}

// Export variable to envp
void export_var(char ***envp_ref, const char *assignment) {
    char **envp = *envp_ref;
    char *equal = strchr(assignment, '=');
    if (!equal) {
        fprintf(stderr, "export: invalid format\n");
        return;
    }

    // Extract name part from assignment
    char name[256];
    strncpy(name, assignment, equal - assignment);
    name[equal - assignment] = '\0';

    int index = find_var_index(envp, name);
    if (index != -1) {
        // Replace value: free old and assign new
        free(envp[index]);
        envp[index] = strdup(assignment);
    } else {
        // Add new variable
        int count = env_count(envp);
        char **new_env = malloc(sizeof(char *) * (count + 2)); // +1 for new var, +1 for NULL

        for (int i = 0; i < count; i++)
            new_env[i] = envp[i];  // Move pointers only

        new_env[count] = strdup(assignment);
        new_env[count + 1] = NULL;

        free(envp); // free only the array of pointers, not the strings
        *envp_ref = new_env;
    }
}

// Unset variable from envp
void unset_var(char ***envp_ref, const char *name) {
    char **envp = *envp_ref;
    int index = find_var_index(envp, name);
    if (index == -1)
        return;

    int count = env_count(envp);
    char **new_env = malloc(sizeof(char *) * count); // one less slot
    int j = 0;
    for (int i = 0; i < count; i++) {
        if (i == index) {
            free(envp[i]); // free the string value
            continue;
        }
        new_env[j++] = envp[i];
    }
    new_env[j] = NULL;

    free(envp); // free old array
    *envp_ref = new_env;
}

// Print the environment
void print_env(char **envp) {
    for (int i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
}

// Free all env variables
void free_env(char **envp) {
    for (int i = 0; envp[i]; i++)
        free(envp[i]);
    free(envp);
}
int main(int ac, char **av, char **envp) {
    (void)ac;
    (void)av;

    // Step 1: Duplicate envp into editable memory
    int count = env_count(envp);
    char **my_env = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++)
        my_env[i] = strdup(envp[i]);
    my_env[count] = NULL;

    // Step 2: Modify env
    export_var(&my_env, "EDITOR=nano");
    export_var(&my_env, "USER=SusuShell");
    unset_var(&my_env, "OLDPWD");

    // Step 3: Show results
    printf("Updated env:\n");
    print_env(my_env);

    // Step 4: Clean up
    free_env(my_env);

    return 0;
}

