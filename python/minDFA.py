from dfa import *
from praut import *

def pairFR(L):
    return list(map(lambda x: ((L[0], x), -1), L[1:]))

def state_combos(L):
    """L is assumed to have a length of >= 2 during normal usage
    """
    if len(L) <= 2:
        return([((L[0], L[1]), -1)])
    else:
        return (pairFR(L)) + (state_combos(L[1:]))

def sepFinNonFin(ht, D):
    # kv is a key-value pair; notice that a key is a state pair (q0,q1)
    # and a value is a number (-1, 0, 1, ...) representing the
    # distinguishing relation
    #
    sepfn = lambda x,y: x in D["F"] and y in (D["Q"] - D["F"])
    #
    for kv in ht.items():
        if sepfn(kv[0][0], kv[0][1]) or sepfn(kv[0][1], kv[0][0]):
            # Distinguish those states separated by sepfn
            ht[kv[0]] = 0

def fixptDist(ht, D):
    """Iterate through ht till no changes happen to ht.
    """
    changed = True
    while changed:
        changed = False
        for kv in ht.items():
            s0 = kv[0][0]
            s1 = kv[0][1]
            for c in D["Sigma"]:
                ns0 = D["Delta"][(s0,c)]
                ns1 = D["Delta"][(s1,c)]
                #
                # Distinguishable states carry a "distance" info
                if ns0 == ns1:
                    continue
                if (ns0, ns1) in ht:
                    if ht[(s0,s1)] == -1 and ht[(ns0, ns1)] >= 0:
                        ht[(s0,s1)] = ht[(ns0, ns1)] + 1
                        changed = True
                        break
                else:
                    if (ns1, ns0) in ht:
                        if ht[(s0,s1)] == -1 and ht[(ns1, ns0)] >= 0:
                            ht[(s0,s1)] = ht[(ns1, ns0)] + 1
                            changed = True
                            break
                    else:
                        print("ht does not cover all required state combos.")
    return ht
        

def bash_eql_classes(eql_reln):
    """Eql reln is [(a,b),(a,c),(d,e),(f,h),(g,f),..].
    Grow eql classes and pad rep. Return [(a,[eqa]), (b,[eqb]), ...].
    """
    print("Bashing")
    print(eql_reln)
    return bash_1(eql_reln, [])

def listminus(L1, L2):
    return [x for x in L1 if x not in L2]

def bash_1(eql_reln, L_eq_classes):
    """L_eq_classes is a list of sets that are the eqlt classes.
    """
    if eql_reln == []:
        return mk_rep_eqc(L_eq_classes)
    else:
        eq0 = eql_reln[0]
        # a and b are eqlt. merge into L_eq_classes
        a = eq0[0]
        b = eq0[1]
        SaL = [Sa for Sa in L_eq_classes if a in Sa] #expect this to be a singleton list at most
        SbL = [Sb for Sb in L_eq_classes if b in Sb] #expect this to be a singleton list at most
        #
        # 1. a,b pair is totally new
        if (SaL == [] and SbL == []):
            return bash_1(eql_reln[1:], [{a,b}] + L_eq_classes)
        elif (SbL == [] and not(SaL == [])):
            return bash_1(eql_reln[1:], listminus(L_eq_classes, SaL) + [SaL[0] | {b}])
        elif (SaL == [] and not(SbL == [])):
            return bash_1(eql_reln[1:], listminus(L_eq_classes, SbL) + [SbL[0] | {a}])
        else:
            return bash_1(eql_reln[1:], listminus(L_eq_classes, SaL + SbL) + [SaL[0] | SbL[0]])

def mk_rep_eqc(L_eq_classes):
    Ll = list(map(lambda x: list(x), L_eq_classes))
    return list(map(lambda x: (x[0], x), Ll))


def F_of(F, final_rep_eqc):
    return { x for (x,X) in final_rep_eqc if not (set(F) & set(X)) == set({}) }

def rep_of_s(s, final_rep_eqc):
    if final_rep_eqc == []:
        print("Error, did not find a rep for state s")
    else:
        x_X = final_rep_eqc[0]
        if s in x_X[1]:
            return x_X[0]
        else:
            return q0_of(s, final_rep_eqc[1:])    

def q0_of(q0, final_rep_eqc):
    return rep_of_s(q0, final_rep_eqc)

def Delta_of(Delta, fre):
    return { (rep_of_s(s0, fre), a): rep_of_s(s1, fre)  for  ((s0,a),s1) in Delta.items() }

def mk_state_eqc_name(L):
    return "_".join(L)
        
def minDFA(D, state_name_mode):
    """Given a DFA D, go through the state minimization algorithm.
    state_name_mode is 'verbose' or 'succinct', producing two variants, as you can guess.
    """
    # Build a dict of initial state pairs
    ht = dict(state_combos(list(D["Q"])))
    # Mark final and non-final states to be different
    # We pass a function that distinguishes final and non-final states
    sepFinNonFin(ht, D)
    # print(ht)
    #
    ht = fixptDist(ht, D)
    print("initial ht is..")
    print(ht)
    # Pick out equivalent pairs
    ht_1 = [ a for (a,b) in ht.items() if b == -1 ]
    #
    # Now form equivalence classes
    # what's returned is [ ( [all_eql_states_1], rep_1), ( [all_eql_states_2], rep_2), .. ]
    rep_eqc = bash_eql_classes(ht_1)
    #
    # These are the merged eql. classes; other eql classes are singleton. Combine all.
    #
    eqc_states_singleton = listminus(D["Q"], list({x for (x,y) in ht_1}|{y for (x,y) in ht_1}))
    rep_eqc_1 = [(x, [x]) for x in eqc_states_singleton]
    final_rep_eqc = rep_eqc + rep_eqc_1
    #
    # Must now build a state machine out of final_rep_eqc
    print("final_rep_eqc = ")
    print(final_rep_eqc)
    #
    minQ = {x for (x,y) in final_rep_eqc}
    minSigma = D["Sigma"]
    minq0 = q0_of(D["q0"], final_rep_eqc)
    minF = F_of(D["F"], final_rep_eqc)
    minDelta = Delta_of(D["Delta"], final_rep_eqc)
    #
    if state_name_mode == 'verbose':
        state_rename_ht = { x : mk_state_eqc_name(y) for (x,y) in final_rep_eqc }
        minQ = { state_rename_ht[x] for x in minQ }
        minq0 = state_rename_ht[minq0]
        minF = { state_rename_ht[f] for f in minF }
        minDelta = { (state_rename_ht[x], y) : state_rename_ht[z] for ((x,y),z) in minDelta.items() }
    #
    print("minQ")
    print(minQ)    
    print("minSigma")
    print(minSigma)    
    print("minq0")
    print(minq0)    
    print("minF")
    print(minF)
    print("minDelta")
    print(minDelta)
    return mk_dfa(minQ, minSigma, minDelta, minq0, minF)

#=================================================================

def mk_DFAFig1011():
    Q = {'S1', 'S2', 'S3', 'S4', 'S5', 'S6'}
    Sigma = {'a','b'}
    Delta = {('S1', 'a'): 'S2',
             ('S1', 'b'): 'S3',
             ('S2', 'a'): 'S4',
             ('S3', 'a'): 'S5',
             ('S2', 'b'): 'S5',
             ('S3', 'b'): 'S4',
             ('S5', 'a'): 'S6',
             ('S5', 'b'): 'S6',
             ('S4', 'a'): 'S6',
             ('S4', 'b'): 'S6',
             ('S6', 'a'): 'S6',
             ('S6', 'b'): 'S6'}
    q0 = 'S1'
    F = {'S2', 'S3', 'S6'}
    return mk_dfa(Q, Sigma, Delta, q0, F)


def mk_tree2DFA():
    Q = {'S','Sa', 'Sb', 'Sc', 'Saa', 'Scc'}
    Sigma = {'a','b','c'}
    Delta = {('S', 'a'): 'Sa',
             ('S', 'b'): 'Sb',
             ('S', 'c'): 'Sc',
             ('Sa', 'a'): 'Saa',
             ('Sc', 'c'): 'Scc'}
    q0 = 'S'
    F = {'Saa', 'Scc'}
    return mktot(mkp_dfa(Q, Sigma, Delta, q0, F))

def mk_Qtt():
    Q = {'S','Sa', 'Sb', 'Sc', 'Saa', 'Sab', 'Sac'}
    Sigma = {'a','b','c'}
    Delta = {('S', 'a'): 'Sa',
             ('S', 'b'): 'Sb',
             ('S', 'c'): 'Sc',
             ('Sa', 'a'): 'Saa',
             ('Sa', 'b'): 'Sab',
             ('Sa', 'c'): 'Sac' }
    q0 = 'S'
    F = {'Saa', 'Sab', 'Sac'}
    return mktot(mkp_dfa(Q, Sigma, Delta, q0, F))


def mk_tree3DFA():
    Q = {'S','Sa', 'Sb', 'Sc', 'Saa', 'Sab', 'Sac', 'Sba', 'Sbb', 'Sbc', 'Sca', 'Scb', 'Scc',
          'Saaa', 'Sccc'}
    Sigma = {'a','b','c'}
    Delta = {('S', 'a'): 'Sa',
             ('S', 'b'): 'Sb',
             ('S', 'c'): 'Sc',
             ('Sa', 'a'): 'Saa',
             ('Sa', 'b'): 'Sab',
             ('Sa', 'c'): 'Sac',
             ('Sb', 'a'): 'Sba',
             ('Sb', 'b'): 'Sbb',
             ('Sb', 'c'): 'Sbc',
             ('Sc', 'a'): 'Sca',
             ('Sc', 'b'): 'Scb',
             ('Sc', 'c'): 'Scc',
             ('Saa', 'a'): 'Saaa',
             ('Scc', 'c'): 'Sccc' }
    q0 = 'S'
    F = {'Saaa', 'Sccc'}
    return mktot(mkp_dfa(Q, Sigma, Delta, q0, F))

minDTree3 = minDFA(mk_tree3DFA(), 'verbose')


#---min DFA
# ==> Enable if you wish : dot_dfa(minDTree3, "minDTree3.dot")

#=================================================================






    
    

    
