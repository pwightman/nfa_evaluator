# -----------------------------------------------------------------------------
# re.py
#
# A simple regexp parser that disambiguates over reg_ambiguous.py wrt cat and +
# try a b  + c d then you'll know!

#
# Adapted from calc.py that is available from www.dabeaz.com/ply/example.html
# -----------------------------------------------------------------------------

from nfa import *
import os
import os.path

tokens = (
    'EPS','STR','LPAREN','RPAREN','PLUS','STAR'
    )

# Tokens

t_PLUS    = r'\+'
t_STAR    = r'\*'
t_LPAREN  = r'\('
t_RPAREN  = r'\)'
t_EPS     = r'""'
t_STR     = r'[a-zA-Z_][a-zA-Z0-9_]*'


# Ignored characters
t_ignore = " \t"

def t_newline(t):
    r'\n+'
    t.lexer.lineno += t.value.count("\n")
    
def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)
    
# Build the lexer
import ply.lex as lex
lex.lex()

# Parsing rules

precedence = (
    ('left','PLUS'),
    ('left','STAR'),
    )

def p_expression_plus(t):
    '''expression : expression PLUS catexp'''
    print("-> plus(" + t[1]["q0"] + "," + t[3]["q0"] + ")")
    t[0] = mk_plus_nfa(t[1], t[3])
#--
def mk_plus_nfa(N1, N2):
    q0 = next(St)
    add_delta = { (q0, ""): set({N1["q0"], N2["q0"]}) }
    add_delta.update(N1["Delta"])
    add_delta.update(N2["Delta"])
    return mk_nfa(N1["Q"] | N2["Q"] | set({q0}), N1["Sigma"] | N2["Sigma"], add_delta, q0, N1["F"] | N2["F"])    

def p_expression_plus_id(t):
    '''expression : catexp'''
    print("-> catexp: " + t[1]["q0"])
    t[0] = t[1]
#--
# No need to compute any other attributes        

def p_expression_cat(t):
    '''catexp :  catexp ordyexp'''
    print("-> cat(" + t[1]["q0"] + "," + t[2]["q0"] + ")")
    t[0] = mk_cat_nfa(t[1], t[2])
#--
def mk_cat_nfa(N1, N2):
    add_delta = dict({}) 
    add_delta.update(N1["Delta"])
    add_delta.update(N2["Delta"])
    for f in N1["F"]:
        if (f, "") in N1["Delta"]: # Then union
            add_delta.update({ (f, ""): N1["Delta"][(f, "")] | set({N2["q0"]}) } )
        else:
            add_delta.update({ (f, ""): set({N2["q0"]}) })
    return mk_nfa(N1["Q"] | N2["Q"], N1["Sigma"] | N2["Sigma"], add_delta, N1["q0"], N2["F"] - N1["F"])

def p_expression_cat_id(t):
    '''catexp :  ordyexp'''
    print("-> ordyexpr:" + t[1]["q0"])
    t[0] = t[1]
#--
# No need to compute any other attributes    

def p_expression_ordy_star(t):
    'ordyexp : ordyexp STAR'
    print("-> star(" + t[1]["q0"] + ")")
    t[0] = mk_star_nfa(t[1])
#--
def mk_star_nfa(N):
    add_delta = dict({})
    q0 = next(St)    
    add_delta.update({ (q0,""): set({N["q0"]}) })
    add_delta.update(N["Delta"])
    #
    for f in N["F"]:
        if (f, "") in N["Delta"]:
            add_delta.update({ (f, ""): N["Delta"][(f, "")] | set({N["q0"]}) })
        else:
            add_delta.update({ (f, ""): set({N["q0"]}) })
    #
    return mk_nfa(N["Q"] | set({q0}), N["Sigma"], add_delta, q0, N["F"] | set({q0}))
    
def p_expression_ordy_paren(t):
    'ordyexp : LPAREN expression RPAREN'
    print("-> (" + t[2]["q0"] + ")")
    t[0] = t[2]
#--
# No need to compute any other attributes    

def p_expression_ordy_str(t):
    'ordyexp : STR'
    print("-> string:" + t[1])
    t[0] = mk_symbol_nfa(t[1])
#--
def mk_symbol_nfa(a):
    q0 = next(St)
    f  = next(St)
    return mk_nfa(set({q0,f}), set({a}), { (q0,a): set({f}) }, q0, set({f}))    


def p_expression_ordy_eps(t):
    'ordyexp : EPS'
    print('-> eps.')
    t[0] = mk_eps_nfa()
#--    
def mk_eps_nfa():
    q0 = next(St)
    f  = next(St)
    return mk_nfa(set({q0,f}), set({}), { (q0,""): set({f}) }, q0, set({f}))
    

def p_error(t):
    print("Syntax error at '%s'" % t.value)

import ply.yacc as yacc
yacc.yacc()

#--
def gensym(seed=0):
    n = seed
    while True:
        yield("s"+str(n))
        n += 1

St = gensym()        


#--

def reparse(s):
    dot_nfa(yacc.parse(s), "/tmp/temp.dot")
    if input("display file? [y/n]") == 'y':
        os.system("dot -Tpdf /tmp/temp.dot > /tmp/temp.pdf")
        os.system("open -a Preview /tmp/temp.pdf")
        if input("remove file /tmp/temp.pdf?") == 'y':
            os.system("/bin/rm -rf /tmp/temp.pdf")
        
        
    

    
    
