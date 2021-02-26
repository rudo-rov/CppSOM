parser grammar SOMParser;

options { tokenVocab = SOMLexer; }

classdef:
    Identifier Equal superclass
    instanceFields method*
    ( Separator classFields classMethod* )?
    EndTerm;

superclass:
    Identifier? NewTerm;

instanceFields:
    ( Or variable* Or )?;

classFields:
    ( Or variable* Or )?;

classMethod:
    method;

method:
   pattern Equal ( Primitive | methodBlock );

pattern:
    unaryPattern | keywordPattern | binaryPattern;

unaryPattern:
    unarySelector;

binaryPattern:
    binarySelector argument;

keywordPattern:
    ( keyword argument )+;

methodBlock:
    NewTerm blockContents? EndTerm;

unarySelector:
    identifier;

binarySelector:
    Or | Comma | Minus | Equal | Not | And | Star | Div | Mod | Plus | More |
    Less | At | Per | OperatorSequence;

identifier:
    Primitive | Identifier;

keyword:
    Keyword;

argument:
    variable;

blockContents:
    ( Or localDefs Or )?
    blockBody;

localDefs:
    variable*;

blockBody:
      Exit result
    | expression ( Period blockBody? )?;

result:
    expression Period?;

expression:
    assignation | evaluation;

assignation:
    assignments evaluation;

assignments:
    assignment+;

assignment:
    variable Assign;

evaluation:
    primary messages?;

primary:
    variable | nestedTerm | nestedBlock | literal;

variable:
    identifier;

messages:
      unaryMessage+ binaryMessage* keywordMessage?
    | binaryMessage+ keywordMessage?
    | keywordMessage;

unaryMessage:
    unarySelector;

binaryMessage:
    binarySelector binaryOperand;

binaryOperand:
    primary unaryMessage*;

keywordMessage:
    ( keyword formula )+;

formula:
    binaryOperand binaryMessage*;

nestedTerm:
    NewTerm expression EndTerm;

literal:
    literalArray | literalSymbol | literalString | literalNumber;

literalArray:
    Pound NewTerm
    literal*
    EndTerm;

literalNumber:
    negativeDecimal | literalDecimal;

literalDecimal:
    literalInteger | literalDouble;

negativeDecimal:
    Minus literalDecimal;

literalInteger:
    Integer;

literalDouble:
    Double;

literalSymbol:
    Pound ( string | selector );

literalString:
    string;

selector:
    binarySelector | keywordSelector | unarySelector;

keywordSelector:
    Keyword | KeywordSequence;

string:
    STString;

nestedBlock:
    NewBlock blockPattern? blockContents? EndBlock;

blockPattern:
    blockArguments Or;

blockArguments:
    ( Colon argument )+;
