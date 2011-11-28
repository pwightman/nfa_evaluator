# Dfa operations
from lang import *
from praut import *
from dfa import *


# Many higher-order functions are kept in functools
from functools import reduce

def fst(p):
    """ First of a pair."""
    return p[0]

def snd(p):
    """ Second of a pair."""
    return p[1]

def fn_dom(F):
    """ For functions represented as hash-maps (dicts), return their domain as a set.
    """
    return {k for k in F.keys()}

def fn_range(F):
    """ For functions represented as hash-maps (dicts), return their range as a set.
    """    
    return {v for v in F.values()}

def mk_nfa(Q, Sigma, Delta, q0, F):
    """Make an NFA, doing the consistency checks needed.
    """
    assert(Sigma != {})
    assert("" not in Sigma)  # We don't allow epsilon in any alphabet (except for a GNFA)
    assert(q0 in Q)
    assert(set(F) <= Q)
    assert(fn_dom(Delta) <= product(Q, Sigma | {""}))
    # Delta maps state x input to sets of states...
    # One way to say this
    # assert(fn_range(Delta) <= pow(Q))
    # But this causes trouble as sets can't be members of sets..
    for x in list(Delta.values()):
        assert(set(x) <= Q)
    #
    return({"Q":Q, "Sigma":Sigma, "Delta":Delta, "q0":q0, "F":F})

Q1 = {'S0','S1'}

Sigma1 = {'a','b'}

Delta1 = { ('S0', 'a'): {'S0', 'S1'}, ('S1', 'a'): {'S0'}, ('S1', 'b'): {}, ('S0', ''): {'S1'} }

q01 = 'S0'

F1 = {'S1'}

NFA1 = mk_nfa(Q1, Sigma1, Delta1, q01, F1)

assert(NFA1 == {'Q': {'S1', 'S0'}, 'q0': 'S0', 'F': {'S1'}, 'Sigma': {'a', 'b'}, 'Delta': {('S0', 'a'): {'S1', 'S0'}, ('S1', 'a'): {'S0'}, ('S1', 'b'): {}, ('S0', ''): {'S1'}}})

def mktot_nfa(N):
    """ Given a partially specified NFA, make it total by transitioning to the set of states {} wherever
    a move is undefined.
    """
    add_delta = { (q,c) : set({}) for q in N["Q"] for c in (N["Sigma"] | {""}) if (q,c) not in N["Delta"] }
    #
    add_delta.update(N["Delta"])
    #
    return {"Q": N["Q"], "Sigma": N["Sigma"], "q0": N["q0"], "F": N["F"], "Delta": add_delta}

def prnfa(N):
        """Prints the NFA neatly.
        """
        # Make the NFA total before printing
        Nt = mktot_nfa(N)
        print("")
        print("Q:", Nt["Q"])
        print("Sigma:", Nt["Sigma"])
        print("q0:", Nt["q0"])
        print("F:", Nt["F"])
        print("Delta:")
        print("\t".join(map(str, Nt["Q"])))
        print("----------------------------------------------------------------------------------------")
        for c in (Nt["Sigma"] | {""}):
            nxt_qs = [Nt["Delta"][(q, c)] for q in Nt["Q"]]
            print("\t".join(map(str, nxt_qs)) + "\t\t" + c)
            print("")

def step_nfa(N, q, c):
    """Run NFA N from state q on character c or "". Return the next state.
    Step is without E-closure.
    """
    assert(c in (N["Sigma"] | {""}))
    assert(q in N["Q"])
    # We have to run it wrt the total version of the NFA. Expensive, so special case this check.
    if (q,c) in N["Delta"].keys():
        return N["Delta"][(q,c)]
    else:
        return set({})

#-- Eclosure version 1 ---------------------------------------------------
# def eps_fix(set_states, chain_len, N):
#     if (chain_len == 0):
#         return set_states
#     else:
#         all_state_sets_one_eps_away = list(map(lambda s: step_nfa(N, s, ""), set_states))
#         # basis case added to make reduction succeed        
#         all_states_one_eps_away = reduce(lambda x, y: set(x) | set(y), all_state_sets_one_eps_away + [ set() ] )
#         return eps_fix( set(all_states_one_eps_away) | set(set_states), chain_len - 1, N)
# 
# def eclosure(Q, N):
#     """Given nfa N and a SET OF states Q, close Q, return eclosure as a set of states.
#     """
#     return set() if Q == {} else eps_fix(Q, len(N["Delta"].items()), N)
#------------------------------------------------------------------------

def ech(Allsofar, Previous, N):
    """Extend Allsofar until nothing new (that's not in Previous).
    """
    if (Allsofar == Previous):
        return Allsofar
    else:
        all_state_sets_one_eps_away = list(map(lambda q: step_nfa(N, q, ""), Allsofar))
         # basis case added to make reduction succeed        
        all_states_one_eps_away = reduce(lambda x, y: set(x) | set(y), all_state_sets_one_eps_away + [ set({}) ] )
        return ech(set(all_states_one_eps_away) | set(Allsofar), Allsofar, N)
    
def eclosure(Q, N):
    """Given nfa N and a SET OF states Q, close Q, return eclosure as a set of states.
    """
    return ech(Q, set({}), N)

def ec_step_nfa(Q, c, N):
    """Return all states one "c" step away from Q (E-close before/after). i.e.,
    E-close Q, run N from all those states, Eclose those.
    If c is "", then se simply E-close q.
    len(NFA1["Delta"].items()) is the longest chain in the NFA.. we can iterate that much.
    """
    Eclosure = eclosure(Q, N)
    #
    all_state_sets_one_c_away = list(map(lambda s: step_nfa(N, s, c), Eclosure))
    #
    # basis case added to make reduction succeed
    all_states_one_c_away = reduce(lambda x, y: set(x) | set(y), all_state_sets_one_c_away + [ set({}) ] )
    #
    Eclosure_again = eclosure(all_states_one_c_away, N)
    #
    return Eclosure_again


def run_nfa(N, Q, s):
    """Run NFA from a set of states Q.
    """
    return eclosure(Q, N) if s=="" else run_nfa(N, ec_step_nfa(Q, s[0], N), s[1:])

def accepts_nfa(N, q, s):
    """NFA acceptance.
    """
    return (run_nfa(N, {q}, s) & N["F"]) != set({})

def accepts_nfav(N, q, s):
    """NFA acceptance - verbose version.
    """
    if (run_nfa(N, {q}, s) & N["F"]) != set({}):
        print("NFA accepts '" + s + "' by reaching " + str(run_nfa(N, {q}, s)))
        return True
    else:
        print("NFA rejects '" + s + "'")
        return False

#-- nfa2dfa --    
# FrontQ,     Qsofar,             Delta
# List of SS, List of SS so far,  Function
#
# Initially, Qsofar == set()
# 
# For each c in Sigma,
#   move each item in FrontQ through c
#   see if any outside Qsofar; add those SS to FrontQ and fill Delta with move to SS
#   until all c generate fall into FrontQ
#   Export Q=Qsofar, Sigma, Delta, q0=Init, F = map over Qsofar picking out F in sets
   
def nfa2dfa(N):
    EC = eclosure({N["q0"]}, N)
    # Qsofar is set to [EC] because we have discovered the 'in's to EC; the outs will
    # be added while returning
    return n2d([EC], [EC], dict({}), N)

def n2d(FrontQ, Qsofar, Delta, N):
    # was the idea: unfortun. double call to ec_step_nfa; alt is using Qn but then iter over powerset?
    # NewMoves = [ ((Q, c), ec_step_nfa(Q, c, N)) for Q in FrontQ for c in N["Sigma"] if ec_step_nfa(Q, c, N) not in Qsofar ]
    AllMoves = [ ((Q, c), ec_step_nfa(Q, c, N)) for Q in FrontQ for c in N["Sigma"] ]
    # print("AllMoves")
    # print(AllMoves)
    NewMoves = list(filter(lambda QcQ: QcQ[1] not in Qsofar, AllMoves)) # [1] picks out c of ((a,b),c)
    # print("NewMoves")
    # print(NewMoves)
    if NewMoves == []:
        # print("Must still add last bit to Delta to discover last bit out OUTs")
        AllMovesDelta = dict([ ( (str(Qfrom), c), str(Qto) ) for ((Qfrom, c), Qto) in AllMoves ])
        Delta.update(AllMovesDelta)
        #
        DFA_Q = { str(Q) for Q in Qsofar }
        DFA_Sigma = N["Sigma"]
        DFA_Delta = Delta
        DFA_q0 = str(eclosure({N["q0"]}, N))
        DFA_F = set(map(lambda Q: str(Q), filter(lambda Q: (N["F"] & Q) != set({}), Qsofar)))
        #
        # print("DFA_Q")
        # print(DFA_Q)
        # print("DFA_Sigma")
        # print(DFA_Sigma)
        # print("DFA_Delta")
        # print(DFA_Delta)
        # print("DFA_q0")
        # print(DFA_q0)
        # print("DFA_F")
        # print(DFA_F)
        return mk_dfa(DFA_Q, DFA_Sigma, DFA_Delta, DFA_q0, DFA_F)
    else:
        newFrontQ = list(map(lambda QcQ: QcQ[1], NewMoves)) # picks out c of ((a,b),c)
        newQsofar = Qsofar + newFrontQ
        # was NewMoves; changed to AllMoves below because even the jumps back need to be added
        NewMovesDelta = dict([ ( (str(Qfrom), c), str(Qto) ) for ((Qfrom, c), Qto) in AllMoves ]) 
        Delta.update(NewMovesDelta)
        return n2d(newFrontQ, newQsofar, Delta, N)
    
        
        

#    assert(c in (N["Sigma"] | {""}))
#    assert(q in N["Q"])
#    # We have to run it wrt the total version of the NFA
#    Nt = mktot_nfa(N)
#    return Nt["Delta"][(q,c)]            

ecs_S0_a = ec_step_nfa({'S0'}, 'a', NFA1)
ecs_S0_b = ec_step_nfa({'S0'}, 'b', NFA1)
#
ecs_S1_a = ec_step_nfa({'S1'}, 'a', NFA1)
ecs_S1_b = ec_step_nfa({'S1'}, 'b', NFA1) 
#
ec_S0 = eclosure({'S0'}, NFA1)
ec_S1 = eclosure({'S1'}, NFA1)


Q2 = {'S0', 'S1', 'S2', 'S3', 'S4', 'S5', 'S6'}

Sigma2 = {'0','1'}

Delta2 = { ('S0', '')  : { 'S1' },
           ('S0', '0') : { 'S0' },
           ('S0', '1') : { 'S0', 'S5' },
           ('S1', '0') : { 'S2' },
           ('S2', '0') : { 'S3' },
           ('S2', '1') : { 'S3' },
           ('S3', '0') : { 'S4' },
           ('S3', '1') : { 'S4' },
           ('S5', '0') : { 'S6' },
           ('S5', '1') : { 'S6' } }

q02 = 'S0'

F2 = {'S4', 'S6'}

NFA2 = mk_nfa(Q2, Sigma2, Delta2, q02, F2)




ec_S0 = eclosure({'S0'}, NFA2)
ec_S1 = eclosure({'S1'}, NFA2)
ec_S4 = eclosure({'S4'}, NFA2)

ecs_S0_0 = ec_step_nfa({'S0'}, '0', NFA2)
ecs_S0_1 = ec_step_nfa({'S0'}, '1', NFA2)

ecs_S1_0 = ec_step_nfa({'S1'}, '0', NFA2)
ecs_S1_1 = ec_step_nfa({'S1'}, '1', NFA2)

ecs_S2_0 = ec_step_nfa({'S2'}, '0', NFA2)
ecs_S2_1 = ec_step_nfa({'S2'}, '1', NFA2)



assert(run_nfa(NFA2, {'S0'}, '000') == {'S3', 'S2', 'S1', 'S0', 'S4'})

assert(run_nfa(NFA2, {'S0'}, '00') == {'S3', 'S2', 'S1', 'S0'})

assert(run_nfa(NFA2, {'S0'}, '0') == {'S2', 'S1', 'S0'})

assert(run_nfa(NFA2, {'S0'}, '1') == {'S1', 'S0', 'S5'})

assert(run_nfa(NFA2, {'S0'}, '11') == {'S1', 'S0', 'S6', 'S5'})

assert(run_nfa(NFA2, {'S0'}, '101') == {'S3', 'S1', 'S0', 'S5'})


assert(accepts_nfa(NFA2, 'S0', '101') == False)

assert(accepts_nfa(NFA2, 'S0', '111') == True)

assert(accepts_nfa(NFA2, 'S0', '') == False)

assert(accepts_nfa(NFA2, 'S0', '0') == False)

assert(accepts_nfa(NFA2, 'S0', '00') == False)

assert(accepts_nfa(NFA2, 'S0', '000') == True)

assert(accepts_nfa(NFA2, 'S0', '001') == True)

assert(accepts_nfa(NFA2, 'S0', '011') == True)

assert(accepts_nfa(NFA2, 'S0', '100') == False)

assert(accepts_nfa(NFA2, 'S0', '10') == True)

assert(accepts_nfav(NFA2, 'S0', '10') == True) # prints also


assert(accepts_nfav(NFA2, 'S0', '010') == True) # prints also

assert(accepts_nfav(NFA2, 'S0', '010') == True) # prints also

assert(accepts_nfav(NFA2, 'S0', '0100') == False) # prints also

assert(accepts_nfav(NFA2, 'S0', '01000') == True) # prints NFA accepts '01000' by reaching {'S3', 'S2', 'S1', 'S0', 'S4'}


# ---
#prdfa_nomktot(nfa2dfa(NFA2))
#
#Q: {"{'S1', 'S0', 'S5'}", "{'S1', 'S0', 'S6', 'S5', 'S4'}", "{'S1', 'S0'}", "{'S1', 'S0', 'S6', 'S5'}", "{'S2', 'S1', 'S0', 'S6', 'S4'}", "{'S2', 'S1', 'S0'}", "{'S3', 'S2', 'S1', 'S0'}", "{'S3', 'S1', 'S0', 'S5', 'S4'}", "{'S3', 'S1', 'S0', 'S5'}", "{'S2', 'S1', 'S0', 'S6'}", "{'S3', 'S2', 'S1', 'S0', 'S4'}"}
#Sigma: {'1', '0'}
#q0: {'S1', 'S0'}
#F: {"{'S1', 'S0', 'S6', 'S5', 'S4'}", "{'S3', 'S2', 'S1', 'S0', 'S4'}", "{'S1', 'S0', 'S6', 'S5'}", "{'S2', 'S1', 'S0', 'S6', 'S4'}", "{'S3', 'S1', 'S0', 'S5', 'S4'}", "{'S2', 'S1', 'S0', 'S6'}"}
#Delta:
#{'S1', 'S0', 'S5'}	{'S1', 'S0', 'S6', 'S5', 'S4'}	{'S1', 'S0'}	{'S1', 'S0', 'S6', 'S5'}	{'S2', 'S1', 'S0', 'S6', 'S4'}	{'S2', 'S1', 'S0'}	{'S3', 'S2', 'S1', 'S0'}	{'S3', 'S1', 'S0', 'S5', 'S4'}	{'S3', 'S1', 'S0', 'S5'}	{'S2', 'S1', 'S0', 'S6'}	{'S3', 'S2', 'S1', 'S0', 'S4'}
#----------------------------------------------------------------------------------------
#{'S1', 'S0', 'S6', 'S5'}	{'S1', 'S0', 'S6', 'S5'}	{'S1', 'S0', 'S5'}	{'S1', 'S0', 'S6', 'S5'}	{'S3', 'S1', 'S0', 'S5'}	{'S3', 'S1', 'S0', 'S5'}	{'S3', 'S1', 'S0', 'S5', 'S4'}	{'S1', 'S0', 'S6', 'S5', 'S4'}	{'S1', 'S0', 'S6', 'S5', 'S4'}	{'S3', 'S1', 'S0', 'S5'}	{'S3', 'S1', 'S0', 'S5', 'S4'}	1
#
#{'S2', 'S1', 'S0', 'S6'}	{'S2', 'S1', 'S0', 'S6'}	{'S2', 'S1', 'S0'}	{'S2', 'S1', 'S0', 'S6'}	{'S3', 'S2', 'S1', 'S0'}	{'S3', 'S2', 'S1', 'S0'}	{'S3', 'S2', 'S1', 'S0', 'S4'}	{'S2', 'S1', 'S0', 'S6', 'S4'}	{'S2', 'S1', 'S0', 'S6', 'S4'}	{'S3', 'S2', 'S1', 'S0'}	{'S3', 'S2', 'S1', 'S0', 'S4'}	0
#
#>>> 

DNFA2 = nfa2dfa(NFA2)

G2 = mktot(mk_gnfa(nfa2dfa(NFA2)))

G2_d_S32104 = del_state_from_gnfa(G2, "{'S3', 'S2', 'S1', 'S0', 'S4'}")

G2_d_S2106  = del_state_from_gnfa(mktot(G2_d_S32104), "{'S2', 'S1', 'S0', 'S6'}")

G2_d_S105   = del_state_from_gnfa(mktot(G2_d_S2106), "{'S1', 'S0', 'S5'}")

G2_d_S31054   = del_state_from_gnfa(mktot(G2_d_S105), "{'S3', 'S1', 'S0', 'S5', 'S4'}")

G2_d_S10654   = del_state_from_gnfa(mktot(G2_d_S31054), "{'S1', 'S0', 'S6', 'S5', 'S4'}")

G2_d_S10   = del_state_from_gnfa(mktot(G2_d_S10654), "{'S1', 'S0'}")

G2_d_S1065   = del_state_from_gnfa(mktot(G2_d_S10), "{'S1', 'S0', 'S6', 'S5'}")

G2_d_S21064   = del_state_from_gnfa(mktot(G2_d_S1065), "{'S2', 'S1', 'S0', 'S6', 'S4'}")

G2_d_S210   = del_state_from_gnfa(mktot(G2_d_S21064), "{'S2', 'S1', 'S0'}")

G2_d_S3210   = del_state_from_gnfa(mktot(G2_d_S210), "{'S3', 'S2', 'S1', 'S0'}")

G2_d_S3105   = del_state_from_gnfa(mktot(G2_d_S3210), "{'S3', 'S1', 'S0', 'S5'}")

#prdfa(G2_d_S3105)
#prdfa(G2_d_S3105)
#
#Q: {'Real_F', 'BH', 'Real_I'}
#Sigma: {' ( ( (1)  (1) ) ) (1)*  (EPS +  (0) )  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (0) )  +  ( (0) )  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( (0) (0)*  +  (1 +  (0) (0)*  (1) )  +  ( (1 +  (0) (0)*  (1) )  (1) )  +  ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  (EPS +  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) ) ) ', ' ( ( (1)  (1) ) ) (1)*  (EPS +  (0) )  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (0) )  +  ( (0) )  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( (0) (0)*  +  (1 +  (0) (0)*  (1) )  +  ( (1 +  (0) (0)*  (1) )  (1) )  +  ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  (EPS +  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) ) )  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (1) )  +  ( (0) )  (1)  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (0) )  +  ( (0) )  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (1) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (1) ) ) ( ( (1)  (1) ) (1)*  ( (0)  (1) )  +  (0)  (1)  +  ( ( (1)  (1) ) (1)*  ( (0)  (0) )  +  (0)  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (1) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (1) ) )*  ( (1)  +  ( (1)  (1) ) (1)*  (EPS +  (0) )  +  (0)  +  ( ( (1)  (1) ) (1)*  ( (0)  (0) )  +  (0)  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( (0) (0)*  +  (1 +  (0) (0)*  (1) )  +  ( (1 +  (0) (0)*  (1) )  (1) )  +  ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  (EPS +  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) ) ) ) '}
#q0: Real_I
#F: {'Real_F'}
#Delta:
#Real_F	BH	Real_I
#----------------------------------------------------------------------------------------
#BH	BH	BH	 ( ( (1)  (1) ) ) (1)*  (EPS +  (0) )  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (0) )  +  ( (0) )  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( (0) (0)*  +  (1 +  (0) (0)*  (1) )  +  ( (1 +  (0) (0)*  (1) )  (1) )  +  ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  (EPS +  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) ) ) 
#
#BH	BH	Real_F	 ( ( (1)  (1) ) ) (1)*  (EPS +  (0) )  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (0) )  +  ( (0) )  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( (0) (0)*  +  (1 +  (0) (0)*  (1) )  +  ( (1 +  (0) (0)*  (1) )  (1) )  +  ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  (EPS +  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) ) )  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (1) )  +  ( (0) )  (1)  +  ( ( ( (1)  (1) ) ) (1)*  ( (0)  (0) )  +  ( (0) )  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (1) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (1) ) ) ( ( (1)  (1) ) (1)*  ( (0)  (1) )  +  (0)  (1)  +  ( ( (1)  (1) ) (1)*  ( (0)  (0) )  +  (0)  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (1) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (1) ) )*  ( (1)  +  ( (1)  (1) ) (1)*  (EPS +  (0) )  +  (0)  +  ( ( (1)  (1) ) (1)*  ( (0)  (0) )  +  (0)  (0) ) ( ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  ( (0)  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) )  (0) )*  ( (0) (0)*  +  (1 +  (0) (0)*  (1) )  +  ( (1 +  (0) (0)*  (1) )  (1) )  +  ( ( (1 +  (0) (0)*  (1) )  (1) )  (1) ) (1)*  (EPS +  (0) )  +  ( (1 +  (0) (0)*  (1) )  (0) ) ) ) 
#
#>>> 



Qe = {'S0', 'S1', 'S2', 'S3', 'S4', 'S5'}

Sigmae = {'a','b','c','d'}

Deltae = { ('S0', 'a')  : { 'S1', 'S2' },
           ('S1', 'b') : { 'S4' },
           ('S2', 'd') : { 'S5' },
           ('S3', '') : { 'S0' },
           ('S4', 'c') : { 'S3' },
           ('S4', '') : { 'S2' },
           ('S5', '') : { 'S1' },
           ('S5', 'c') : { 'S0' } }

q0e = 'S0'

Fe = {'S0','S3','S4'}

NFAe = mk_nfa(Qe, Sigmae, Deltae, q0e, Fe)


#-- Midterm Exam 1, Q1, CS 3100, Fall 2011, NFA

Q_q1 = {'S0', 'S1', 'S2', 'S3', 'S4'}

Sigma_q1 = {'a','b'}

Delta_q1 = { ('S0', 'a')  : { 'S2' },
           ('S0', 'b')  : { 'S1' },
           ('S1', 'b')  : { 'S3' },
           ('S2', 'b')  : { 'S4' },
           ('S2', '')   : { 'S1' },
           ('S4', '')   : { 'S0' } }

q0_q1 = 'S0'

F_q1 = {'S2','S3','S4'}

NFA_q1 = mk_nfa(Q_q1, Sigma_q1, Delta_q1, q0_q1, F_q1)

# ==> Enable if you wish!! dot_nfa(NFA_q1, "q1.dot")


#---NFA to DFA

# ==> Enable if you wish: dot_dfa(DNFA2, "DNFA2.dot")


