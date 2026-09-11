// ============================================================================
// list.c -- YOUR WORK for Lab 1.
//
// Implement every function below so it matches its contract in list.h. Each has
// a TODO describing what to do; the header has the ownership rule and Big-O.
//
// The placeholder bodies let the project compile and stay leak-free BEFORE you
// start, so the whole workflow (make, make valgrind, make format) works from
// minute one. As you implement each function, real behaviour appears.
//
// The `(void)x;` lines just tell the compiler "I know x is unused for now" so
// -Werror does not stop you. Delete each one once you use that variable.
// ============================================================================

#include "list.h"

#include <stdlib.h> // you will need malloc and free

TokenList list_create(void) {
    // GIVEN: an empty list is all-null with length 0. (This one is done for you
    // as an example of a designated initializer -- study it, then build the
    // rest below.)
    return (TokenList){.head = nullptr, .tail = nullptr, .length = 0};
}

void list_destroy(TokenList *list) {
    // TODO: walk every node from head. For EACH node, in this order:
    //   1) TokenNode *next = current->next;   // save next FIRST
    //   2) token_free(&current->token);       // free the owned lexeme
    //   3) free(current);                     // free the node box
    //   4) current = next;
    // Then reset the header (e.g. *list = list_create();).
    // Freeing the node before saving ->next is a use-after-free -- ASan catches
    // it, so try it once on purpose to see the report.
    TokenNode *current = list->head;
    while (current != nullptr) {
        TokenNode *next = current->next;
        token_free(&current->token);
        free(current);
        current = next;
    }
    *list = list_create();
}

bool list_push_front(TokenList *list, Token token) {
    // TODO: allocate a node (malloc), store `token` in it, and link it at the
    // FRONT (new node's next = old head; head = new node). If the list was empty
    // (tail == nullptr), the new node is also the tail. Then length++.
    // On malloc failure: token_free(&token); return false;  (do not leak!)
    TokenNode *node = malloc(sizeof *node);
    if (node == nullptr) {
        token_free(&token);
        return false;
    }
    node->token = token;
    node->next  = list->head;
    list->head  = node;
    if (list->tail == nullptr) {
        list->tail = node;
    }
    list->length++;
    return true;
}

bool list_push_back(TokenList *list, Token token) {
    // TODO: allocate a node holding `token` and append it at the BACK. If the
    // list is empty, it becomes both head and tail; otherwise link tail->next to
    // it and move tail. Then length++. Same malloc-failure rule as above.
    TokenNode *node = malloc(sizeof *node);
    if (node == nullptr) {
        token_free(&token);
        return false;
    }
    node->token = token;
    node->next  = nullptr;
    if (list->tail == nullptr) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail       = node;
    }
    list->length++;
    return true;
}

bool list_insert_after(TokenList *list, TokenNode *node, Token token) {
    // TODO: insert a new node holding `token` immediately after `node`. Careful
    // with the two links: new->next = node->next; node->next = new. If `node`
    // was the tail, update tail. Then length++. If `node` is nullptr, or malloc
    // fails, token_free(&token) and return false.
    if (node == nullptr) {
        token_free(&token);
        return false;
    }
    TokenNode *NN = malloc(sizeof *NN);
    if (NN == nullptr) {
        token_free(&token);
        return false;
    }
    NN->token  = token;
    NN->next   = node->next;
    node->next = NN;
    if (list->tail == node) {
        list->tail = NN;
    }
    list->length++;
    return true;
}

TokenNode *list_find_first(const TokenList *list, TokenKind kind) {
    // TODO: walk from head; return the first node whose token.kind == kind, or
    // nullptr if none is found.
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        if (cur->token.kind == kind) {
            return cur;
        }
    }
    return nullptr;
}

bool list_remove_first(TokenList *list, TokenKind kind) {
    // TODO: find the first node with this kind while remembering the node BEFORE
    // it (`prev`). Unlink it (mind the head case where prev is nullptr, and the
    // tail case where you must move tail back to prev). Free its token then the
    // node. length--. Return true if removed, false if not found.
    TokenNode *prev = nullptr;
    TokenNode *cur  = list->head;
    while (cur != nullptr) {
        if (cur->token.kind == kind) {
            if (prev == nullptr) {
                list->head = cur->next;
            } else {
                prev->next = cur->next;
            }
            if (list->tail == cur) {
                list->tail = prev;
            }
            token_free(&cur->token);
            free(cur);
            list->length--;
            return true;
        }
        prev = cur;
        cur  = cur->next;
    }
    return false;
}

size_t list_length(const TokenList *list) {
    // TODO: return the length you maintain in the header.
    return list->length;
    return 0;
}

void list_print(const TokenList *list, FILE *out) {
    // TODO: for each node, print one line to `out`. Suggested format (matches
    // the reference so grading scripts line up):
    //   fprintf(out, "  %-9s '%s' (%d:%d)\n",
    //           token_kind_name(cur->token.kind), cur->token.lexeme,
    //           cur->token.line, cur->token.col);
    for (const TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        fprintf(out, "  %-9s '%s' (%d:%d)\n", token_kind_name(cur->token.kind), cur->token.lexeme,
                cur->token.line, cur->token.col);
    }
}

void list_to_dot(const TokenList *list, FILE *out) {
    // TODO (do this LAST, after the list works): print Graphviz DOT to `out`.
    // A minimal version is fine to start:
    //   digraph tokens { rankdir=LR; node [shape=record];
    //     n0 [label="{KIND|lexeme}"]; n0 -> n1; ... last -> nnull; }
    // See list.h and the README for what the picture should look like.
    fprintf(out, "digraph tokens {\n");
    fprintf(out, "  rankdir=LR;\n");
    fprintf(out, "  node [shape=record];\n");

    size_t i = 0;
    for (const TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        fprintf(out, "  n%zu [label=\"{%s|", i, token_kind_name(cur->token.kind));
        for (const char *p = cur->token.lexeme; *p != '\0'; p++) {
            if (*p == '{' || *p == '}' || *p == '|' || *p == '<' || *p == '>' || *p == ' ') {
                fputc('\\', out);
            }
            fputc(*p, out);
        }
        fprintf(out, "}\"];\n");
        if (cur->next != nullptr) {
            fprintf(out, "  n%zu -> n%zu;\n", i, i + 1);
        }
        i++;
    }

    fprintf(out, "}\n");
}

bool list_check_invariant(const TokenList *list) {
    // TODO: return true only if the list is well formed:
    //   - length == number of nodes reachable from head
    //   - tail is the last node, and is nullptr exactly when the list is empty
    // Returning true here is a placeholder; implement it so it can actually
    // catch a bookkeeping mistake for you.
    size_t counted        = 0;
    const TokenNode *cur  = list->head;
    const TokenNode *last = nullptr;
    while (cur != nullptr) {
        last = cur;
        counted++;
        cur = cur->next;
    }
    if (counted != list->length) {
        return false;
    }
    if (list->tail != last) {
        return false;
    }
    return true;
}
