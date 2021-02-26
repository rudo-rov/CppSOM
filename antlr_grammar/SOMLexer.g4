lexer grammar SOMLexer;

Comment:   '"' ~["]* '"' -> skip;
Whitespace : [ \t\r\n]+ -> skip ;
Primitive: 'primitive';
Identifier: [\p{Alpha}] [\p{Alpha}0-9_]*;
Equal: '=';
Separator: '----' '-'*;
NewTerm: '(';
EndTerm: ')';
Or: '|';
Comma: ',';
Minus: '-';
Not:   '~';
And:   '&';
Star:  '*';
Div:   '/';
Mod:   '\\';
Plus:  '+';
More:  '>';
Less:  '<';
At:    '@';
Per:   '%';
OperatorSequence: (
    Not | And | Or | Star | Div |
    Mod | Plus | Equal | More | Less |
    Comma | At | Per | Minus )+;
Colon: ':';
NewBlock: '[';
EndBlock: ']';
Pound:  '#';
Exit:   '^';
Period: '.';
Assign: ':=';
Integer: [0-9]+;
Double: [0-9]+ '.' [0-9]+;
Keyword: Identifier Colon;
KeywordSequence: Keyword+;
STString:
    '\''
    (   '\\t'
      | '\\b'
      | '\\n'
      | '\\r'
      | '\\f'
      | '\\0'
      | '\\\''
      | '\\\\'
      |  ~('\''| '\\')
    )*
    '\'';