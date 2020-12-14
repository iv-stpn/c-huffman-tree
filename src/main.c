#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct Node
{
    int letter;                //  Contained letter in (= -1 if no letter is contained)
    int size;                  //  Weight of the node
    struct Node *left, *right; //  Pointers to the nodes below this Node
    char *code;
    struct Element *element_ref;
} Node;

typedef struct Element
{
    int letter;           //  Contained letter
    struct Element *next; //  Pointer to the next element
    int occ;              //  Number of occurrences of the letter
    Node *node;           //  Associated Node structure
} Element;

typedef struct HuffmanTree
{
    Node *root_node;
    Element *root_dict;
} HuffmanTree;

char *NaN = "NaN";
char *_NULL = "NULL";
char *NEWLINE = "LF/NL";
char *TAB = "TAB";
char *SPACE = "SPACE";
char *DEL = "DEL";
char *ALT = "ALT";
char *SPECIAL = "CHR";

char fgetc_ascii(FILE *file)
{
    char chr = fgetc(file);
    int curr_char = (int)chr;
    if (curr_char < -1 || curr_char > 255)
    {
        curr_char = 0;
    }
    chr = (char)curr_char;
    return chr;
}

char *display_char(char chr)
{
    int c = (int)chr;
    char *new_chr = NULL;

    switch (c)
    {
    case 0:
        new_chr = _NULL;
        break;

    case 10:
        new_chr = NEWLINE;
        break;

    case 9:
        new_chr = TAB;
        break;

    case 32:
        new_chr = SPACE;
        break;

    case 127:
        new_chr = DEL;
        break;

    case 255:
        new_chr = ALT;
        break;

    default:
        if (c < 0 || c > 255)
        {
            new_chr = NaN;
        }
        else if (c < 32)
        {
            new_chr = malloc(strlen(SPECIAL) + 2);
            sprintf(new_chr, "%s%02d", SPECIAL, c);
        }
        else
        {
            new_chr = malloc(2*sizeof(char));
            new_chr[0] = (char)c;
            new_chr[1] = '\0';
        }
    }

    return new_chr;
}


Element *get_element(Element *root, int index)
{
    if (index >= 0)
    {
        Element *curr = root;
        for (int i = 0; i < index; i++)
        {
            if (curr->next != NULL)
            {
                curr = curr->next;
            }
            else
            {
                exit(EXIT_FAILURE);
            }
        }
        return curr;
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}

char get_char_from_display_char(char *display_chr)
{
    if (strlen(display_chr) == 1)
    {
        return *display_chr;
    }
    else if (strlen(display_chr) == 0)
    {
        return -1;
    }
    else
    {
        if (strcmp(display_chr, NaN)  == 0)
        {
            return -1;
        }
        else if (strcmp(display_chr, _NULL) == 0)
        {
            return 0;
        }
        else if (strcmp(display_chr, NEWLINE) == 0)
        {
            return 10;
        }
        else if (strcmp(display_chr, TAB) == 0)
        {
            return 9;
        }
        else if (strcmp(display_chr, SPACE) == 0)
        {
            return 32;
        }
        else if (strcmp(display_chr, DEL) == 0)
        {
            return 127;
        }
        else if (strcmp(display_chr, ALT) == 0)
        {
            return 255;
        }
        else if (strlen(display_chr) == strlen(SPECIAL) + 2 && strncmp(SPECIAL, display_chr, strlen(SPECIAL)) == 0)
        {
            int chr;
            sscanf(display_chr, "%s%02d", ((char*)0), &chr);
            return ((char)chr);
        }
    }
}

void print_node(Node *node)
{
    Node *left = node->left;
    Node *right = node->right;

    if (left != NULL)
    {
        if (left->letter == -1)
        {
            printf("(%d) <- ", left->size);
        }
        else
        {
            printf("(%s, %d) <- ", display_char(left->letter), left->size);
        }
    }
    else
    {
        printf("  NULL  <- ");
    }

    if (node->letter == -1)
    {
        printf("(%d)", node->size);
    }
    else
    {
        printf("(%s, %d)", display_char(node->letter), node->size);
    }

    if (right != NULL)
    {
        if (right->letter == -1)
        {
            printf(" -> (%d)", right->size);
        }
        else
        {
            printf(" -> (%s, %d)", display_char(right->letter), left->size);
        }
    }
    else
    {
        printf(" ->  EOF");
    }
}

void print_element(Element *element, int display_code, int elem_or_node)
{
    if (elem_or_node)
    {
        if (display_code)
        {
            printf("-> %s: %d (%s)\n", display_char(element->node->letter), element->node->size, element->node->code);
        }
        else
        {
            printf("-> %s: %d\n", display_char(element->node->letter), element->node->size);
        }
    }
    else
    {
        if (display_code)
        {
            printf("-> %s: %d (%s)\n", display_char(element->letter), element->occ, element->node->code);
        }
        else
        {
            printf("-> %s: %d\n", display_char(element->letter), element->occ);
        }
    }
}

void print_occurrences(Element *root, int display_code, int elem_or_node)
{
    if (root == NULL)
    {
        printf("Error: The linked list of occurrences is empty.");
        return;
    }
    printf("\n");
    Element *curr = root;
    do
    {
        print_element(curr, display_code, elem_or_node);
        curr = curr->next;
    } while (curr != NULL);

    printf("-> EOF\n");
}

// C : Fonction qui renvoie une liste contenant chaque caractere present dans le texte,
//     ainsi que le nombre d’occurrences de ce caractere.

int idx_letter(Element *root, char letter)
{
    if (root != NULL)
    {
        Element *curr = root;
        for (int i = 0; curr != NULL; i++)
        {
            if (curr->letter == letter)
            {
                return i;
            }
            curr = curr->next;
        }
    }
    return -1;
}

Node *new_node(int letter, int occ, Node *left, Node *right, Element *element_ref)
{
    Node *node = malloc(sizeof(Node));
    node->letter = letter;
    node->size = occ;
    node->left = left;
    node->right = right;
    node->code = "";
    node->element_ref = element_ref;
}

Element *new_element(int letter)
{
    Element *new = malloc(sizeof(Element));
    new->next = NULL;
    new->letter = letter;
    new->occ = 1;
    new->node = new_node(-1, -1, NULL, NULL, new);

    return new;
}

// To pass an entire array to a function, only the name of the array is passed as an argument
Element *get_occurrences(char *text)
{
    int text_length = strlen(text), idx_occ;
    if (text_length > 0)
    {
        Element *root = new_element(text[0]);
        Element *curr = root;
        for (int i = 1; i <= text_length - 1; i++)
        {
            idx_occ = idx_letter(root, text[i]);
            if (idx_occ == -1)
            {
                curr->next = new_element(text[i]);
                curr = curr->next;
            }
            else
            {
                get_element(root, idx_occ)->occ++;
            }
        }
        return root;
    }
    return NULL;
}

// D : Fonction qui renvoie un arbre de Huffman, à partir d’une liste d’occurrences

char *insert_char_in_front(char c, char *curr_string)
{
    int new_len = strlen(curr_string) + 1;
    char *new_string = malloc(new_len * sizeof(char));
    new_string[0] = c;

    for (int i = 1; i <= new_len; i++)
    {
        if (i == new_len) {
            new_string[i] = '\0';
        } else {
            new_string[i] = curr_string[i - 1];
        }
    }

    return new_string;
}

void swap(Element *a, Element *b)
{
    Element *temp = malloc(sizeof(Element));

    temp->letter = a->letter;
    temp->occ = a->occ;
    temp->node = a->node;

    a->letter = b->letter;
    a->occ = b->occ;
    a->node = b->node;
    a->node->element_ref = a;

    b->letter = temp->letter;
    b->occ = temp->occ;
    b->node = temp->node;
    b->node->element_ref = b;
}

void new_compare(int *comparer_curr, int *comparer_to_insert, Element *curr, Element *to_insert, int node_or_elem,
                 int size_or_letter)
{
    if (size_or_letter)
    {
        if (node_or_elem)
        {
            *comparer_curr = curr->letter;
            *comparer_to_insert = to_insert->letter;
        }
        else
        {
            *comparer_curr = curr->node->letter;
            *comparer_to_insert = to_insert->node->letter;
        }
    }
    else
    {
        if (node_or_elem)
        {
            *comparer_curr = curr->occ;
            *comparer_to_insert = to_insert->occ;
        }
        else
        {
            *comparer_curr = curr->node->size;
            *comparer_to_insert = to_insert->node->size;
        }
    }
}

void insert_elem(Element *to_insert, Element *after, int insert_before)
{
    Element *temp = after->next;
    after->next = to_insert;
    to_insert->next = temp;
    if (insert_before)
    {
        swap(after, to_insert);
    }
}

Element *insert_elem_desc(Element *root, Element *to_insert, int node_or_elem, int size_or_letter, int verbose)
{
    if (verbose)
    {
        printf("\n*** insert_elem_desc (%s, %s) ***", (node_or_elem ? "elem" : "node"), (size_or_letter ? "letter" : "size"));
    }

    Element *curr = root;
    int comparer_curr, comparer_to_insert;
    new_compare(&comparer_curr, &comparer_to_insert, curr, to_insert, node_or_elem, size_or_letter);

    if (comparer_to_insert > comparer_curr)
    {
        if (verbose)
        {
            if (node_or_elem)
            {
                printf("\nto_insert (%s, %d) greater than root (%s, %d): inserting and swapping", display_char(to_insert->letter),
                       to_insert->occ, display_char(root->letter), root->occ);
            }
            else
            {
                printf("\nto_insert [");
                print_node(to_insert->node);
                printf("] greater than root [");
                print_node(root->node);
                printf("]: inserting and swapping");
            }
        }
        insert_elem(to_insert, root, 1);
        if (verbose)
        {
            printf("\n************************\n");
        }
        return root;
    }
    else
    {
        while (comparer_curr > comparer_to_insert)
        {
            if (curr->next == NULL)
            {
                if (node_or_elem)
                {
                    printf("\nreached the end of the occurrences: inserting at the end");
                }
                else
                {
                    printf("\nreached the end of the occurrences: inserting at the end");
                }
                curr->next = to_insert;
                if (verbose)
                {
                    printf("\n************************\n");
                }
                return to_insert;
            }
            else
            {
                if (verbose)
                {
                    if (node_or_elem)
                    {
                        printf("\nto_insert (%s, %d) smaller than curr (%s, %d): continuing", display_char(to_insert->letter),
                               to_insert->occ, display_char(curr->letter), curr->occ);
                    }
                    else
                    {
                        printf("\nto_insert [");
                        print_node(to_insert->node);
                        printf("] smaller than [");
                        print_node(curr->node);
                        printf("]: continuing");
                    }
                }
                curr = curr->next;
                new_compare(&comparer_curr, &comparer_to_insert, curr, to_insert, node_or_elem, size_or_letter);
            }
        }
        if (verbose)
        {
            if (node_or_elem)
            {
                printf("\nto_insert (%s, %d) greater than or equal to curr (%s, %d): inserting", display_char(to_insert->letter), to_insert->occ,
                       display_char(curr->letter), curr->occ);
            }
            else
            {
                printf("\nto_insert [");
                print_node(to_insert->node);
                printf("] greater than or equal to curr [");
                print_node(curr->node);
                printf("]: inserting");
            }
        }
        insert_elem(to_insert, curr, 1);
        if (verbose)
        {
            printf("\n************************\n");
        }
        return to_insert;
    }
}

int SPACING = 10;
// Displays a tree with the leaves first, going back to the root
void print_tree_2D(Node *root, int space)
{
    if (root == NULL)
        return;

    space += SPACING;

    // Process right children first
    print_tree_2D(root->right, space);

    // Display the current node
    printf("\n");
    for (int i = SPACING; i < space; i++)
        printf(" ");

    if (root->letter == -1)
    {
        printf("(%d)\n", root->size);
    }
    else
    {
        printf("(%s, %d)\n", display_char(root->letter), root->size);
    }

    // Process left children afterwards
    print_tree_2D(root->left, space);
}

void print_tree_2D_wrapper(struct Node *root)
{
    print_tree_2D(root, 0);
}

// G : fonction qui compresse un fichier texte
//     Le fichier d’entree ne sera pas modifie, un autre fichier, contenant le texte compresse sera cree

int len(Element *root)
{
    Element *curr = root;
    int i;
    for (i = 1; curr->next != NULL; i++)
    {
        curr = curr->next;
    }
    return i;
}

// Add occurrences to a new tab (kept in descending order) by dichotomy
Element *get_occurrences_by_dichotomy(FILE *input_file, int verbose)
{
    if (verbose)
    {
        printf("\n*** get_occurrences_by_dichotomy ***\n");
    }

    char curr_char = fgetc_ascii(input_file);

    // Create and initialize the tab
    Element *root;
    if (curr_char == EOF)
    {
        printf("Error: file is empty.");
        exit(EXIT_FAILURE);
    }
    else
    {
        root = new_element(curr_char);
        if (verbose)
        {
            printf("Starting with character %s, adding with occurence: %d\n", display_char(curr_char), 1);
        }
        curr_char = fgetc_ascii(input_file);
    }

    int lower_bnd, upper_bnd, middle;
    Element *curr;

    int curr_len = 1;
    while (curr_char != EOF)
    {
        lower_bnd = 0;
        upper_bnd = curr_len - 1;
        middle = (upper_bnd + lower_bnd) / 2;
        curr = get_element(root, middle);
        if (verbose)
        {
            printf("\nSearching for the next letter: %s (%d) (lower bound: %d, upper bound: %d)", display_char(curr_char), curr_char, lower_bnd, upper_bnd);
        }
        while (lower_bnd < upper_bnd)
        {
            if (verbose)
            {
                printf("\nSearching... lower: %d, upper: %d, middle: %d", lower_bnd, upper_bnd, middle);
                printf("\nCurrent element: ");
                print_element(curr, 0, 0);
            }

            if (curr_char > curr->letter)
            {
                upper_bnd = middle - 1;
            }
            else if (curr_char < curr->letter)
            {
                lower_bnd = middle + 1;
            }
            else
            {
                lower_bnd = middle;
                upper_bnd = middle; //End the while loop
            }
            if (verbose)
            {
                printf("\nNearest index hasn't yet been reached; current index (idx: %d, letter: %s)\n", middle,
                       display_char(curr->letter));
                printf("New boundaries: (%d, %d)\n", lower_bnd, upper_bnd);
            }
            middle = (upper_bnd + lower_bnd) / 2;
            curr = get_element(root, middle);
        }

        if (verbose)
        {
            printf("\nNearest index reached: %d", middle);
            printf("\nCurrent element: ");
            print_element(curr, 0, 0);
        }
        if (curr->letter == curr_char)
        {
            if (verbose)
            {
                printf("Letter %s already exists, new occurence: %d\n", display_char(curr_char), curr->occ + 1);
            }
            curr->occ++;
        }
        else
        {
            if (verbose)
            {
                printf("Letter %s isn't in the list, adding with occurence: %d\n", display_char(curr_char), 1);
            }

            if (curr_char < curr->letter)
            {
                insert_elem(new_element(curr_char), curr, 0);
            }
            else
            {
                insert_elem(new_element(curr_char), curr, 1);
            }
            //insert_elem_desc(root, new_element(curr_char), 1, 1, 1);
            curr_len++;
        }

        curr_char = fgetc_ascii(input_file);
    }
    fseek(input_file, 0, SEEK_SET);

    if (verbose)
    {
        printf("\ngot occurrences:");
        print_occurrences(root, 0, 0);
        printf("\n***********************************\n");
    }

    return root;
}

// J : Fonction qui trie un tableau de noeuds en fonction des occurrences

// C Implementation of the Quick Sorting Algorithm for Linked List
void quick_sorting(Element *root, int first, int last)
{
    int pivot;
    if (first < last)
    {
        pivot = first; //On definit le pivot au debut
        int curr_pivot_occ = get_element(root, pivot)->occ;

        int i = first, j = last;
        while (i < j)
        {
            // On cherche un element plus grand que le pivot a gauche
            while ((get_element(root, i)->occ >= curr_pivot_occ) && (i < last))
            {
                i++;
            }

            // On cherche un element plus petit que le pivot a droite
            while (get_element(root, j)->occ < curr_pivot_occ)
            {
                j--;
            }

            // Si les deux éléments existent, on les échange
            if (i < j)
            {
                swap(get_element(root, i), get_element(root, j));
            }
        }

        swap(get_element(root, pivot), get_element(root, j));
        quick_sorting(root, first, j - 1); // On repete l'operation jusqu'a obtenir les deux sub-tables
        quick_sorting(root, j + 1, last);
    }
}

void quick_sorting_wrapper(Element *root)
{
    quick_sorting(root, 0, len(root) - 1);
}

void new_code(char zero_or_one, Node *node)
{
    node->code = insert_char_in_front(zero_or_one, node->code);
}

void propagate_new_code(char zero_or_one, Node *node)
{
    new_code(zero_or_one, node);
    if (node->left != NULL)
    {
        propagate_new_code(zero_or_one, node->left);
    }

    if (node->right != NULL)
    {
        propagate_new_code(zero_or_one, node->right);
    }
}

Element *copy_elems_shallow_nodes(Element *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        Element *new_root = new_element(root->letter);
        new_root->occ = root->occ;
        new_root->node = root->node;
        new_root->next = copy_elems_shallow_nodes(root->next);
        return new_root;
    }
}

HuffmanTree *huffman_tree_from_occurrences(Element *root)
{
    quick_sorting_wrapper(root);
    Element *curr = root;
    curr->node = new_node(curr->letter, curr->occ, NULL, NULL, curr);

    while (curr->next != NULL)
    {
        curr = curr->next;
        curr->node = new_node(curr->letter, curr->occ, NULL, NULL, curr);
    }

    Element *dict = copy_elems_shallow_nodes(root);

    int curr_len = len(root);
    while (curr_len > 1)
    {
        Element *first = get_element(root, curr_len - 1);
        Element *second = get_element(root, curr_len - 2);

        propagate_new_code('0', first->node);
        propagate_new_code('1', second->node);

        Element *sum_elem = malloc(sizeof(Element));
        sum_elem->letter = -1;
        sum_elem->occ = -1;
        sum_elem->node = new_node(-1, first->node->size + second->node->size, first->node, second->node, sum_elem);
        insert_elem_desc(root, sum_elem, 0, 0, 1);

        get_element(root, curr_len - 2)->next = NULL;
        first->next = NULL;
        second->next = NULL;
        curr_len = curr_len - 1;
    }
    HuffmanTree *huffman_tree = malloc(sizeof(HuffmanTree));
    huffman_tree->root_dict = dict;
    huffman_tree->root_node = root->node;

    return huffman_tree;
}

char *delim = ": ";
char *separator = "\n";

void write_huffman_dict(FILE *dict_file, Element *dict)
{
    Element *curr_elem = dict;
    char *buffer;
    char *curr_char;
    char *curr_code;
    int length;

    char *string_format = malloc(sizeof(char) * (4 + strlen(delim) + strlen(separator)));
    sprintf(string_format, "%s%s%s%s", "%s", delim, "%s", separator);

    if (dict_file != NULL && curr_elem != NULL)
    {
        while (curr_elem != NULL)
        {

            curr_char = display_char(curr_elem->letter);
            curr_code = curr_elem->node->code;
            length = strlen(curr_char) + strlen(delim) + strlen(curr_code) + strlen(separator);

            buffer = malloc(length * sizeof(char));
            sprintf(buffer, string_format, curr_char, curr_code);
            fwrite(buffer, 1, length, dict_file);

            curr_elem = curr_elem->next;
        }
        fseek(dict_file, 0, SEEK_SET);
    }
}

FILE *open_file(char *file, char *mode)
{
    FILE *input_file = fopen(file, mode);
    if (input_file == NULL)
    {                                                  // Si fopen n'arrive pas a ouvrir le fichier
        printf("Error: input file cannot be opened."); // Message d'erreur
        exit(EXIT_FAILURE);                            // Arrêt du programme
    }
    return input_file;
}

int char_to_binary(int word)
{
    int res = 0, i = 1;
    while (word > 0)
    {
        //printf("%d %d %d\n", word % 2 , i, word);
        res = res + (i * (word % 2));
        word = word / 2;
        i = i * 10;
    }
    return res;
}

int nb_char_in_file(FILE *input_file, int verbose)
{
    int cpt = 0;
    char curr_char;
    if (verbose)
    {
        printf("\n*** nb_char_in_file ***\n");
    }
    if (input_file != NULL)
    {
        do
        {
            curr_char = fgetc(input_file);
            if (curr_char == EOF)
            {
                printf("Reached end of file.\n");
            }
            else
            {
                cpt++;
                if (verbose)
                {
                    printf("%s (%d)\n", display_char(curr_char), cpt);
                }
            }
        } while (curr_char != EOF);
    }
    fseek(input_file, 0, SEEK_SET);
    if (verbose)
    {
        printf("Got %d chars.", cpt);
        printf("\n***********************\n");
    }
    return cpt;
}

void file_to_binary_file(FILE *input_file, FILE *output_file, int verbose)
{
    if (input_file != NULL && output_file != NULL)
    {
        if (verbose)
        {
            printf("\n*** file_to_binary_file ***\n");
            printf("In binary: ");
        }

        char chr;
        int bin;
        do
        {
            chr = fgetc_ascii(input_file);
            bin = char_to_binary((int)chr);
            if (verbose)
            {
                printf("\n%s: %08d", display_char(chr), bin);
            }
            fprintf(output_file, "%08d", bin);
        } while (chr != EOF);

        fseek(input_file, 0, SEEK_SET);
        fseek(output_file, 0, SEEK_SET);
        if (verbose)
        {
            printf("\n***************************\n");
        }
    }
}

Element *find_elem_in_dict(Element *dict, char *value, int letter_or_code)
{
    Element *curr_elem = dict;
    if (curr_elem != NULL)
    {
        while (curr_elem != NULL)
        {
            if (letter_or_code && strcmp(curr_elem->node->code, value) == 0)
            {
                return curr_elem;
            }
            else if (!letter_or_code && curr_elem->letter == value[0])
            {
                return curr_elem;
            }
            else
            {
                curr_elem = curr_elem->next;
            }
        }
    }
    else
    {
        return NULL;
    }
    return NULL;
}

void compress_file(FILE *input, FILE *output, HuffmanTree *huffman_tree, int verbose)
{
    if (input != NULL)
    {
        char curr_char;
        char *curr_code;
        Element *curr_elem;
        do
        {
            curr_char = fgetc_ascii(input);
            if (curr_char == EOF)
            {
                if (verbose)
                {
                    printf("Reached end of file.\n");
                }
            }
            else
            {
                curr_elem = find_elem_in_dict(huffman_tree->root_dict, &curr_char, 0);
                curr_code = curr_elem->node->code;
                fwrite(curr_code, 1, strlen(curr_code), output);

                if (verbose)
                {
                    printf("Current character: %s (writing %s)\n", display_char(curr_char), curr_code);
                }
            }
        } while (curr_char != EOF);

        fseek(input, 0, SEEK_SET);
        fseek(output, 0, SEEK_SET);
    }
    else
    {
        printf("Error: could not read input file");
        exit(EXIT_FAILURE);
    }
}

void compress_file_wrapper(FILE *input, FILE *output)
{
    Element *occurrences = get_occurrences_by_dichotomy(input, 0);
    HuffmanTree *huffman_root = huffman_tree_from_occurrences(occurrences);

    compress_file(input, output, huffman_root, 0);
}

char *load_full_file(FILE *file)
{
    char *buffer = 0;
    int length;

    if (file)
    {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, file);
        }
        fclose(file);
    }

    if (buffer)
    {
        return buffer;
    }
    else
    {
        return NULL;
    }
}

int n_lines(char *buffer, char *line_separator)
{
    char first = line_separator[0];
    int i, j, count = 0;

    if (first == '\0')
        return strlen(buffer) + 1;

    for (i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == first)
        {
            for (j = 1; line_separator[j] != '\0' && buffer[i + j] == line_separator[j]; j++)
                continue;
            if (line_separator[j] == '\0')
                count++;
        }
    }
    return count;
}

char *strtokm(char *str, const char *delim)
{
    static char *tok;
    static char *next;
    char *m;

    if (delim == NULL) return NULL;

    tok = (str) ? str : next;
    if (tok == NULL) return NULL;

    m = strstr(tok, delim);

    if (m) {
        next = m + strlen(delim);
        *m = '\0';
    } else {
        next = NULL;
    }

    return tok;
}

Element *decode_dict(FILE *input_dictionary, int verbose)
{
    Element *root_elem = NULL;
    Element* curr_elem;
    if (input_dictionary != NULL)
    {
        char *file_buffer = load_full_file(input_dictionary);
        int number_of_lines = n_lines(file_buffer, separator);

        char *lines[number_of_lines];

        char *token = strtokm(file_buffer, separator);
        for (int i = 0; token != NULL; i++)
        {
            lines[i] = token;
            token = strtokm(NULL, separator);
        }

        char *curr_line = lines[0];
        char *curr_char = strtokm(curr_line, delim);
        char *curr_code = strtokm(NULL, delim);
        int line = 1;

        if (curr_line == NULL)
        {
            if (verbose)
            {
                printf("\nUnexpected input: dictionary input is empty.\n");
            }
            return NULL;
        }
        else if (curr_char == NULL || curr_code == NULL)
        {
            if (verbose)
            {
                printf("\nUnexpected input: dictionary input is wrongly formatted (line %d).\n", line);
            }
            return NULL;
        }
        else
        {
            int first_iter = 1;

            root_elem = new_element(get_char_from_display_char(curr_char));
            root_elem->node->code = curr_code;
            curr_elem = root_elem;
            do
            {
                if (curr_char == NULL || curr_code == NULL)
                {
                    if (verbose)
                    {
                        printf("\nUnexpected input: dictionary input is wrongly formatted (line %d).\n", line);
                        return root_elem;
                    }
                }
                else
                {
                    if (!first_iter)
                    {
                        curr_elem->next = new_element(get_char_from_display_char(curr_char));
                        curr_elem->next->node->code = curr_code;
                        curr_elem = curr_elem->next;
                    }
                    else
                    {
                        first_iter = 0;
                    }

                    if (verbose)
                    {
                        printf("Got character: %s with code %s\n", curr_char, curr_code);
                    }
                }
                line++;
                curr_line = lines[line - 1];
                curr_char = strtokm(curr_line, delim);
                curr_code = strtokm(NULL, delim);
            } while (curr_line != NULL && curr_line[0] != 0);
        }
        fseek(input_dictionary, 0, SEEK_SET);
    }
    else
    {
        printf("Error: input dictionary is empty.");
        return NULL;
    }

    return root_elem;
}

void uncompress_file(FILE *input_compressed, FILE *input_dictionary, FILE *output_uncompressed, int verbose)
{
    Element *huffman_dict = decode_dict(input_dictionary, verbose);

    int max_size = strlen(get_element(huffman_dict, len(huffman_dict)-1)->node->code);
    char *curr_code = malloc(sizeof(char)*max_size);
    curr_code[0] = '\0';
    char curr_char;

    if (input_compressed != NULL)
    {
        Element *curr_elem;
        do
        {
            curr_char = fgetc_ascii(input_compressed);
            if (curr_char == EOF)
            {
                if (verbose)
                {
                    printf("Reached end of file.\n");
                }
            }
            else
            {
                strncat(curr_code, &curr_char, 1);
                curr_elem = find_elem_in_dict(huffman_dict, curr_code, 1);
                if (curr_elem != NULL)
                {
                    if (verbose)
                    {
                        printf("Found code: %s (writing %s)\n", curr_code, display_char(curr_elem->letter));
                    }
                    fwrite(&(curr_elem->letter), 1, 1, output_uncompressed);
                    curr_code[0] = '\0';
                }
            }
        } while (curr_char != EOF);
        fseek(input_compressed, 0, SEEK_SET);
    }
    else
    {
        printf("Error: could not read input file");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    FILE *input = open_file("input.txt", "r");
    FILE *output = open_file("output.txt", "w+");

    FILE *output_huffman = open_file("output_huffman.txt", "w+");
    FILE *output_uncompressed = open_file("output_uncompressed.txt", "w+");
    FILE *dict = open_file("dict.txt", "w+");

    file_to_binary_file(input, output, 0);

    //int nb_char_input = nb_char_in_file(input, 0);
    //int nb_char_output = nb_char_in_file(output, 0);

    Element *occurrences = get_occurrences_by_dichotomy(input, 0);
    print_occurrences(occurrences, 0, 0);
    HuffmanTree *huffman_root = huffman_tree_from_occurrences(occurrences);

    print_tree_2D_wrapper(huffman_root->root_node);
    print_occurrences(huffman_root->root_dict, 1, 1);
    write_huffman_dict(dict, huffman_root->root_dict);
    
    compress_file(input, output_huffman, huffman_root, 0);
    uncompress_file(output_huffman, dict, output_uncompressed, 0);

    fclose(input);
    fclose(output);
    fclose(output_huffman);
    fclose(output_uncompressed);
    fclose(dict);

    return EXIT_SUCCESS;
}