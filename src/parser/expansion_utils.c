
#include "minishell.h"

char *expand_value(mem_arena *env_arena, char *token_val, t_env *env, int exit_status)
{
    int	i;
	int	single_quotes;
	int	double_quotes;
    char *result;
	char	*status;
	char	*var_name;
	int	len;
	t_env *node;

	if (!token_val || !env_arena)
		return (NULL);
	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	result = arena_strdup(env_arena, ""); //initialize it to empty string
	if (!result)
		return (NULL);
    while (token_val[i])
    {
        handle_quote_flags(token_val[i], &single_quotes, &double_quotes);
        if (token_val[i] == '$' && !single_quotes)
        {
            i++;
            if (token_val[i] == '?') //exit status variable we get it in the main loop of the program after executing commands
            {
                status = arena_itoa(env_arena, exit_status); //needs to be a string to be able to append to result
				if (!status)
					return (NULL);
                result = ar_strjoin(env_arena, result, status);
                if (!result)
                    return (NULL);
                i++;
            }
            else
            {
                var_name = get_variable_name(env_arena, &token_val[i], &len);
                if (var_name) //variable name is valid
                {
                    node = get_env_node(env, var_name);
                    if (node && node->value)
                        result = ar_strjoin(env_arena, result, node->value); //append value: $USER becomes $nuria
                    else
                        result = ar_strjoin(env_arena, result, ""); // $NOTDEFINED becomes $ (mimics bash behaviour prints nothing, not an error)
				    if (!result)
					    return (NULL);
                    i += len; //skip chars of variable name
                }
                else //invalid variable name, string kept literally
                {
                    result = ar_add_char_to_str(env_arena, result, '$'); // manually append $ so it stays in the result
			        if (!result)
				        return (NULL);
			    }
            }
        }
        else
        {
            result = ar_add_char_to_str(env_arena, result, token_val[i]); // append normal characters if the token contains something like "Hello $USER"
		    if (!result)
			    return (NULL);
            i++;
        }
    }
    return (result);
}



