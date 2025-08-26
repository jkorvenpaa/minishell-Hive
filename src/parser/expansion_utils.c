
#include "minishell.h"

static char *handle_exit_status(t_expansion *data, int *i)
{
    char    *result;

    result = arena_itoa(data->env_arena, data->exit_status); //needs to be a string to be able to append to result
    if (result)
        (*i)++;
    return (result);
}

static char    *resolve_variable(char *token_value, int *i, t_expansion *data)
{
    char    *result;
    int len;
    char *var_name;
    t_env   *node;

    result = NULL;
    (*i)++;
    if (token_value[*i] == '?') // exit status variable we get it in the main loop of the program after executing commands
        result = handle_exit_status(data, i);
    else
    {
        var_name = get_variable_name(data->env_arena, &token_value[*i], &len);
        if (var_name) //variable name is valid
        {
            node = get_env_node(data->env, var_name);
            if (node && node->value)
                result = node->value; //append value: $USER becomes $nuria
            else
                result = ""; // $NOTDEFINED becomes $ (mimics bash behaviour prints nothing, not an error)
            (*i) += len; //skip chars of variable name
        }
        else //invalid variable name, string kept literally
            result = "$";
    }
    return (result);
}

static char *handle_expansion_char(t_expansion *data, char *result, char *token_value, int *i)
{
    char    *new_result;
    char    *exp_value;

    exp_value = resolve_variable(token_value, i, data);
    if (!exp_value)
        return (NULL);
    new_result = ar_strjoin(data->env_arena, result, exp_value);
    if (!new_result)
        return (NULL);
    return (new_result);
}

static char    *expansion_loop(char *token_value, t_expansion *data)
{
    int i;
    int single_quotes;
    int double_quotes;
    char    *result;

    i = 0;
    single_quotes = 0;
    double_quotes = 0;
    result = NULL;
    while (token_value[i])
    {
        handle_quote_flags(token_value[i], &single_quotes, &double_quotes);
        if (token_value[i] == '$' && !single_quotes)
            result = handle_expansion_char(data, result, token_value, &i);
        else
        {
            result = ar_add_char_to_str(data->env_arena, result, token_value[i]); // append normal characters 
            if (!result)
                return (NULL);
            i++;
        }
    }
    return (result);
}

char *expand_value(char *token_val, t_expansion *data)
{
    char *result;

	if (!data || !data->env_arena || !token_val)
		return (NULL);
    result = expansion_loop(token_val, data);
    return (result);
}
