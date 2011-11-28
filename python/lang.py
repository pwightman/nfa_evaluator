# DFA routines
# Author: Ganesh, 8/3/11

def Phi():
    """This is the ZERO language for concatenation (viewed as multiplication)
    """
    return set() # Don't write it as {} because this is ambiguous - dict/set 

def Unit():
    """This is the UNIT language for concatenation (viewed as multiplication)
    """
    return {""} # Set with epsilon

def cat(L1,L2):
    """Concat two languages
    cat(a,b) --> set(['abab', 'bc22', 'ab11', 'ab22', 'bcab', 'bc11']) where a=set(['ab', 'bc']) b=set(['11', 'ab', '22'])
    """
    return set({x+y for x in L1 for y in L2})

def exp(L,n):
    """Exponentiate a language
    exp(a,2) --> set(['abab', 'bcab', 'bcbc', 'abbc']) where  a = set(['ab', 'bc'])
    """
    return Unit() if n == 0 else cat(L, exp(L, n-1))

def star(L,n):
    """Star a language, bounding the iteration to the given n
    star(a,2) --> set(['abab', 'bcbc', 'ab', 'abbc', '', 'bc', 'bcab']), where  a = set(['ab', 'bc'])
    """
    return Unit() if n == 0 else exp(L,n) | star(L,n-1)

def revs(S):
    """Reverse a string - from http://love-python.blogspot.com/2008/02/reverse-string-in-python.html
       revs('ab') --> 'ba'
    """
    return S[::-1]

def revl(L):
    """Reverse a language.
       revl(set(['ab', 'bc'])) --> set(['cb', 'ba'])
    """
    return set(map(lambda x: revs(x), L))

def homos(S,f):
    """String homomorphism wrt lambda f
       homos("abcd",hm) --> 'bcde'  where hm = lambda x: chr( (ord(x)+1) % 256 )
    """
    return "".join(map(f,S))

def homol(L,f):
    """Language homomorphism wrt lambda f.
       homos("Hello there", rot13) --> 'Uryy|-\x81ur\x7fr'
       where rot13 = lambda x: chr( (ord(x)+13) % 256 )
    """
    return set(map(lambda S: homos(S,f), L))
    
def pow(S):
    """Powerset of a set L. Since sets/lists are unhashable, we convert the set to a list,
    perform the powerset operations, leaving the result as a list (can't convert back to a set).
    pow(set(['ab', 'bc'])) --> [['ab', 'bc'], ['bc'], ['ab'], []]
    """
    L=list(S)
    if L==[]:
        return([[]])
    else:
        pow_rest0 = pow(L[1:])
        pow_rest1 = list(map(lambda ls: [L[0]]+ls, pow_rest0))
        return(pow_rest0 + pow_rest1)
    
def lunion(L1,L2):
    """Language union
    """
    return L1 | L2

def lint(L1,L2):
    """Language intersection
    """
    return L1 & L2

def lsymdiff(L1,L2):
    """Language symmetric difference.
      lsymdiff(a,b) where a=set(['ab', 'bc']) b=set(['11', 'ab', '22']) --> set(['11', '22', 'bc'])
    """
    return L1 ^ L2

def lminus(L1,L2):
    """Language subtraction. Can do it as L1.difference(L2) also. 
    """
    return L1 - L2

def lissubset(L1,L2):
    """Language L1 is a subset of L2. Can do it as L1.issubset(L2) also.
    """
    return L1 <= L2

def lissuperset(L1,L2):
    """Language L1 is a supserset of L2. Can do it as L1.issuperset(L2) also.
       lissuperset(set(['ab', 'bc']), set(['ab'])) --> true
    """
    return L1 >= L2
    
def lcomplem(L,alph,m):
    """Complement L relative to alphabset alph. alph is also given as a set of strings.
       We subtract from the "star up to m" of the alphabet alph, the language L.
    """
    return star(alph,m) - L

def product(S1,S2):
    """Cartesian product of two given sets.
    """
    return { (x,y) for x in S1 for y in S2 }

def dotsan_map(x):
    """A homomorphism. We need to sanitize set() also. Ugh!
    """
    if x in {  "{",   " ",   "'",   "}" }: 
        return ""
    elif x == ",":
        return "_"
    elif x == "(":
        return "L"
    elif x == ")":
        return "R"
    else:
        return x

def dot_san_str(S):
    """Make dot like strings which are in set of states notation.
    """
    return homos(S, dotsan_map)

# test lists
L1 = {"a", "b"}
L2 = {"1", "2", "3"}
L3 = {"hello there", "how", "are", '"you"', "doin'?"}

# test homomorphism

hm = lambda x: chr( (ord(x)+1) % 256 )

rot13 = lambda x: chr( (ord(x)+13) % 26 )

# Some tests
# L3 = set(['how', "doin'?", 'hello there', '"you"', 'are'])    
# exp(L3,2)
# set(['arehow', 'howhow', "doin'?how", "doin'?doin'?", 'are"you"', 'howare', "aredoin'?", "howdoin'?", '"you""you"', 'areare', 'how"you"', 'hello therehello there', 'doin\'?"you"', '"you"are', "doin'?hello there", 'hello therehow', "doin'?are", '"you"doin\'?', 'howhello there', '"you"how', 'arehello there', '"you"hello there', 'hello there"you"', 'hello thereare', "hello theredoin'?"])
# >>> star(L3,2)
# set(['', 'arehow', 'hello therehow', '"you"hello there', 'howhow', "doin'?how", '"you"', "hello theredoin'?", "doin'?doin'?", 'hello there', "doin'?", 'are', 'are"you"', 'howhello there', '"you"how', 'howare', "aredoin'?", '"you"doin\'?', "howdoin'?", 'arehello there', '"you""you"', 'how', 'areare', 'how"you"', 'hello therehello there', "doin'?are", 'doin\'?"you"', 'hello there"you"', 'hello thereare', '"you"are', "doin'?hello there"])
# >>> len(exp(L3,2))
# 25
# >>> len(star(L3,2))
# 31
# >>> len(exp(L3,1))
# 5
# >>> len(exp(L3,0))
# 1

# import lang1
# dir(lang1)
# from lang1 import *

# Some fun languages

Lreg = { "a"*i + "b"*j for i in range(4) for j in range(4) }

Lijk = { "a"*i + "b"*j + "c"*k for i in range(4) for j in range(4) for k in range(4) if ((i != 1) | (j == k)) }

Lcsl = { "a"*i + "b"*j + "c"*k for i in range(4) for j in range(4) for k in range(4) if ((i==j) & (j==k)) }

Lodd = { "a"*i for i in range(10) if (i%2 == 1) }

# Reverse of the above

list(map(lambda x: x[::-1], Lijk))

# Reverses 'abc'
list(reversed('abc'))

# Reverses 'abc' because it picks elements from 'abc' with a negative stride of 1 i.e. right-to-left .
# Strided positives are the norm e.g. 'abcd'[::2] gives ac
'abc'[::-1]

# All 0,1 strings within length 5
exp({"0","1"}, 5)

# List of 5-long strings - all of odd length
list(filter(lambda x: (len(x) % 2 == 1),  exp({"0","1"}, 5)))

# List of N-long strings over 0,1 which are odd in length and under 5 in length
list(filter(lambda x: (len(x) % 2 == 1),  star({"0","1"}, 5)))

# clarify {} set() set({}) etc

from math import *

def nthnumeric(N):
    """Assume that Sigma is {a,b}. Produce the Nth string in numeric order, where N >= 0.
    Idea : Given N, get b = floor(log_2(N+1)) - need that many places; what to
    fill in the places is the binary code for N - (2^b - 1) with 0 as a and 1 as b.    
    """
    if(N==0):
        return ''
    else:
        width = floor(log(N+1, 2))
        tofill = int(N - pow(2, width) + 1)
        relevant_binstr = bin(tofill)[2::] # strip the 0b leading string
        len_to_makeup = width - len(relevant_binstr)
        return "a"*len_to_makeup + homos(relevant_binstr, lambda x: 'b' if x=='1' else 'a')

def nthnumeric01(N):
    """Assume that Sigma is {0,1}. Produce the Nth string in numeric order, where N >= 0.
    Idea : Given N, get b = floor(log_2(N+1)) - need that many places; what to
    fill in the places is the binary code for N - (2^b - 1) with 0 as a and 1 as b.    
    """
    if(N==0):
        return ''
    else:
        width = floor(log(N+1, 2))
        tofill = int(N - pow(2, width) + 1)
        relevant_binstr = bin(tofill)[2::] # strip the 0b leading string
        len_to_makeup = width - len(relevant_binstr)
        return "0"*len_to_makeup + relevant_binstr
    
