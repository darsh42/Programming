#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

enum general_tokens {
    ESCAPE_SEQUENCE_START = '\\',

    /* character class */
    CHARACTER_START = '[',
    CHARACTER_END   = ']',

    /* assertions */
    INPUT_START = '^',
    INPUT_END   = '$',

    /* groups and backrefrences*/
    ASSOCIATION_START = '(',
    ASSOCIATION_END   = ')',

    /* word */
    WILDCARD_ZERO_OR_MORE = '*',
    WILDCARD_ONE_OR_MORE = '+',
    WILDCARD_ZERO_OR_ONE = '?',

    QUANTIFIER_START = '{',
    QUANTIFIER_END   = '}',
};

enum character {
    RANGE = '-',
    WILDCARD_ANY_CHAR = '.',
    NEGATION = '^',
    DISJUNCTION = '|',

    /* Escape Sequences */
    DIGIT          = 'd',
    NOT_DIGIT      = 'D',
    WORD           = 'w',
    NOT_WORD       = 'w',
    SPACE          = 's',
    NOT_SPACE      = 'S',
    TAB            = 't',
    CARRIGE_RETURN = 'r',
    LINEFEED       = 'n',
    VERTICAL_TAB   = 'v',
    FORM_FEED      = 'f',
    BACKSPACE      = 'b',
    BACKSLASH      = '\\',
};

enum assertions {
    WORD_BOUNDARY = 'b',
    NON_WORD_BOUNDARY = 'B',

    LOOK = '?',
};

enum quantifiers {

}

struct engine {

};

#endif // ENGINE_H_INCLUDED
