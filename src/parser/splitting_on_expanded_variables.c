

#include "minishell.h"

/**
 * Creates a new WORD token from a substring of a given string.
 * Returns a pointer to the new token, NULL if allocation fails.
 */
static t_token	*new_word_token(mem_arena *arena, char *s, int start, int len)
{
	char	*word;
	t_token	*token;

	word = ar_substr(arena, s, start, len);
	if (!word)
		return (NULL);
	token = create_token_node(arena, word, WORD);
	if (!token)
		return (NULL);
	token->was_quoted = 0;
	token->next = NULL;
	return (token);
}

/**
 * Splits the value of a token into multiple tokens, one for each word.
 * Returns the head of the new list of word tokens, or NULL if allocation fails.
 */
static t_token *split_token_words(mem_arena *arena, t_token *token)
{
	int	start;
	int	end;
	t_token	*head;
	t_token	*new_token;
	
	start = 0;
	head = NULL;
	while (get_next_word(token->value, &start, &end))
	{
		new_token = new_word_token(arena, token->value, start, end - start);
		if (!new_token)
			return (head);
		append_token_to_list(&head, new_token);
		start = end;
	}
	return (head);
}

/**
 * Replaces a single token in a linked list with a list of tokens.
 * Inserts the new list of tokens (split_tokens) in place of the original
 * current token in the linked list. It also updates the previous token,
 * and connects the end of the new list to the remainder of the original one.
 */
static void	replace_token_list(t_token **head, t_token **prev, t_token *current, t_token *split_tokens)
{
	t_token	*tail;

	tail = split_tokens;
	while (tail->next)
		tail = tail->next;
	if (*prev)
		(*prev)->next = split_tokens;
	else
		*head = split_tokens;
	tail->next = current->next;
	*prev = tail;
}

/**
 * Main entry point for splitting.
 * Iterates through a token list and splits all tokens that require it.
 * Returns the head of the updated token list.
 */
t_token	*split_expanded_variables(mem_arena *arena, t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	t_token *head;
	t_token *split_tokens;

	current = tokens;
	prev = NULL;
	head = tokens;
	while (current)
	{
		if (current->type == WORD && needs_splitting(current))
		{
			split_tokens = split_token_words(arena, current);
			if (split_tokens)
				replace_token_list(&head, &prev, current, split_tokens);
			else
				prev = current; //keep original token (not splitted, useful for if VAR="") to avoid infinite loop or crash
		}
		else
			prev = current;
		current = prev->next;
	}
	return (head);
}

/**
 * Removes tokens that have an empty value and were not quoted.
 * Used after variable expansion and word splitting.
 * Updates the linked list to skip removed tokens.
 */
t_token	*remove_empty_tokens(t_token *token)
{
	t_token	*current;
	t_token	*prev;
	t_token *head;

	head = token;
	prev = NULL;
	current = token;
	while (current)
	{
		if (current->value && is_only_spaces(current->value) && !current->was_quoted) //value exists, but it's an empty string and outside quotes (empty quoted tokens are kept)
		{
			if (prev)
				prev->next = current->next;
			else
				head = current->next;
			current = current->next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (head);
}
