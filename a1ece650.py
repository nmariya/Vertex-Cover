#!/usr/bin/env python3
from __future__ import print_function
import sys
import re
import numpy as np 
from scipy.spatial import distance
from numpy import *
import warnings
import itertools

# YOUR CODE GOES HERE
def parse_input(line):
    b = line.split(" ")
    # a1=line
    if (b[0] == "gg\n" or b[0] == "gg"):
        #b = a1.split(" ")
        l = len(b)
        cmd = b[0].strip()
        if l != 1:
            msg = "errorg"
            command = "none"
            street_name = "none"
            coor = "none"
            raise Exception("Syntax should be gg")
        else:
            msg = "accepted"
            command = cmd.strip()
            street_name = "none"
            coor = "none"
    elif (b[0] == "add" or b[0] == "mod" or b[0] == "rm"):
        a=line.rstrip()
        b = a.split("\"")
        while("" in b):
            b.remove("")
        l = len(b)
        cmd = b[0]
        street = b[1]
        #print(cmd)
        if cmd.strip() == "add" or cmd.strip() == "mod":
            if l != 3:
                msg = "errora"
                command = "none"
                street_name = "none"
                coor = "none"
                raise Exception("Syntax should be add/mod \"<street_name>\" (x1,y1)(x2,y2)...")
            elif l == 3:
                #cor_list =  []
                cord = b[-1]
                cord_striped = cord.strip().replace(" ","")
                #cor_list.append(cord_striped)
                x = re.findall(r"\(", cord_striped)
                y = re.findall(r"\)", cord_striped)
                lx = len(x)
                ly = len(y)
                if cmd[-1] != " ":
                    msg = "errora"
                    command = "none"
                    street_name = "none"
                    coor = "none"
                    raise Exception("Syntax should be add/mod \"<street_name>\" (x1,y1)(x2,y2)...")
                elif re.match(r'\S', cord):
                    msg = "errora"
                    command = "none"
                    street_name = "none"
                    coor = "none"
                    raise Exception("Syntax should be add/mod \"<street_name>\" (x1,y1)(x2,y2)...")
                elif lx != ly:
                    msg = "errora"
                    command = "none"
                    street_name = "none"
                    coor = "none"
                    raise Exception("Syntax should be add/mod \"<street_name>\" (x1,y1)(x2,y2)...")
                elif lx == ly:
                    cord_list = re.split(r'\(|\)', cord_striped)
                    while("" in cord_list):
                        cord_list.remove("")
                    #print("cord_list", cord_list)
                    if len(cord_list) <= 1:
                        msg = "errora"
                        command = "none"
                        street_name = "none"
                        coor = "none"
                        raise Exception("Syntax should be add/mod \"<street_name>\" (x1,y1)(x2,y2)...")
                    else:
                        lst = []
                        for i in range(len(cord_list)):
                            c = cord_list[i]
                            c1 = c.split(",")
                            while("" in c1):
                                c1.remove("")
                            d = len(c1)
                            lst.append(d)
                        s_lst = set(lst)
                        if len(s_lst) == 1 and s_lst.pop() == 2:
                            msg = "accepted"
                            command = cmd.strip()
                            street_name = street
                            coor = cord_list
                        else:
                            msg = "errora"
                            command = "none"
                            street_name = "none"
                            coor = "none"
                            raise Exception("Syntax should be add/mod \"<street_name>\" (x1,y1)(x2,y2)...")
                # else:
                #     msg = "accepted"
                #     command = cmd.strip()
                #     street_name = street
                #     coor = cor_list
        elif cmd.strip() == "rm":
            if l != 2:
                msg = "errorr"
                command = "none"
                street_name = "none"
                coor = "none"
                raise Exception("Syntax should be rm \"<street_name>\"")
            elif l == 2:
                if cmd[-1] != " ":
                    msg = "errorr"
                    command = "none"
                    street_name = "none"
                    coor = "none"
                    raise Exception("Syntax should be rm \"<street_name>\"")
                else:
                    msg = "accepted"
                    command = cmd.strip()
                    street_name = street
                    coor = "none"
        else:
            msg = "error_invalid"
            command = "none"
            street_name = "none"
            coor = "none"
            raise Exception("Invalid command")
    else:
        msg = "error_invalid"
        command = "none"
        street_name = "none"
        coor = "none"
        raise Exception("Invalid command")
    return msg, command, street_name, coor

def proper_coordinates(coor):
    res = list(map(eval, coor))
    re = [list(ele) for ele in res] 
    return re

def street_database(db, cmd, street, coor):
    if cmd == "mod":
        if not db:
            raise Exception("Database empty")
        else:
            for k,v in db.items():
                k1=k.lower()
                # print(k1)
                # print(street.lower)
                if street.lower() == k1:
                    db[k] = coor
                    f = 0
                    break
                elif street.lower() != k1:
                    f = 1
                    continue
            if f == 1:
                raise Exception("'mod' specified for a street that does not exist.")
                

    elif cmd == "add":
        for k,v in db.items():
            k=k.lower()
            if street.lower() == k:
                raise Exception("Street already exists")
            elif coor == v:
                raise Exception("Coordinates already exist")
        db[street] = coor
            
    elif cmd == "rm":
        if not db:
            raise Exception("Database empty")
        else:
            for k,v in db.items():
                k1=k.lower()
                if street.lower() == k1:
                    del db[k]
                    f = 0
                    break
                elif street.lower() != k1:
                    f = 1
                    continue
            if f == 1:
                raise Exception("'rm' specified for a street that does not exist.")
                
    elif cmd == "gg":
        return db

    return db

def perp(a) :
    b = empty_like(a)
    b[0] = -a[1]
    b[1] = a[0]
    return b
def seg_intersect(a,b,c,d) :
    a1=np.array(a)
    a2=np.array(b)
    b1=np.array(c)
    b2=np.array(d)
    da = a2-a1
    db = b2-b1
    dp = a1-b1
    dap = perp(da)
    denom = dot( dap, db)
    num = dot(dap, dp)
    X=(num / denom.astype(float))*db + b1
    if (isBetween(a,b,X)):
        if (isBetween(c,d,X)):
            return X
    # else:
    #     return None
def find_vertices(db,V):
    lt = []
    for i in db.values():
        lt.append(i)
    #print(lt)

    inter=[]
    vertices=[]
    intersections=[]
    for j in range(len(lt)-1):
        length=len(lt[j])
        for k in range(length-1):
            a=lt[j][k]
            b=lt[j][k+1]
            for j1 in range(len(lt)):
                if j1 != j:
                    for q in range(len(lt[j1])-1):
                        c=lt[j1][q]
                        d=lt[j1][q+1]
                        #print(a,b,c,d)
                        intersection = seg_intersect(a,b,c,d)
                        if intersection is not None:
                            data = [v for v in intersection if not isnan(v) and not isinf(v)]
                            if data:
                                inter.append(data)
                                vertices.append(float(i) for i in a)
                                vertices.append(float(i) for i in b)
                                vertices.append(float(i) for i in c)
                                vertices.append(float(i) for i in d)
    b_set = set(tuple(x) for x in inter)
    intersections = [ list(np.round_(x,decimals=2)) for x in b_set ]
    b_set = set(tuple(x) for x in vertices)
    vertices = [ list(x) for x in b_set ]
    #print("Intersections :",intersections)
    # print("Vertices :",vertices)
    all_vertices = intersections + vertices
    #print(all_vertices)
    X = V
    w=[]
    for i in range(len(all_vertices)):
        X = {key:val for key, val in X.items() if val != all_vertices[i]}
    for v in X.values():
        w.append(v)
    for i in range(len(w)):
        V = {key:val for key, val in V.items() if val != w[i]}
    if not V:
        w1=0
    else:
        w1 = list(sorted(V.keys()))[-1]
    for j in range(len(all_vertices)):
        if all_vertices[j] not in V.values():
            #print(all_vertices[j])
            V[w1+j]=all_vertices[j]
    return V, intersections

def isBetween(a1, a2, a3):
    a=np.array(a1)
    b=np.array(a2)
    c=np.array(a3)

    crossproduct = (c[1] - a[1]) * (b[0] - a[0]) - (c[0] - a[0]) * (b[1] - a[1])
    
    # compare versus epsilon for floating point values, or != 0 if using integers
    if (round(crossproduct) !=0):
        return False

    dotproduct = (c[0] - a[0]) * (b[0] - a[0]) + (c[1] - a[1])*(b[1] - a[1])
    if dotproduct < 0:
        return False

    squaredlengthba = (b[0] - a[0])*(b[0] - a[0]) + (b[1] - a[1])*(b[1] - a[1])
    if dotproduct > squaredlengthba:
        return False

    return True

def find_edges(db, intersections, V, E):
    lt = []
    for i in db.values():
        lt.append(i)
    edg=[]
    for j in range(len(lt)):
        length=len(lt[j])
        for k in range(length-1):
            a=lt[j][k]
            b=lt[j][k+1]
            intr=[]
            points=[]
            flag=0
            for w in range(len(intersections)):
                c=intersections[w]
                if isBetween(a,b,c):
                    #print(a,b,c)
                    intr.append(c)
                    #print("True",intr)
                    #edg.append()
                    flag=flag + 1
                #else:
                    #print("False")
            if flag == 1:
                #print("a")
                edg1=[]
                edg2=[]
                edg1.append(a)
                edg1 = edg1+intr
                edg2=edg2+intr
                edg2.append(b)
                if (edg1[0][0]!=edg1[1][0] or edg1[0][1]!=edg1[1][1]):
                    edg.append(edg1)
                if (edg2[0][0]!=edg2[1][0] or edg2[0][1]!=edg2[1][1]):
                    edg.append(edg2)
                #print("edg",edg)
            
            if flag > 1:
                dist_d={}
                #a,b,intr
                #print("b")
                points.append(a)
                points=points+intr
                stop_at = len(intr)
                points.append(b)
                t = list(set(map(tuple,points)))
                dist_list=[]
                points = [list(ele) for ele in t]
                count=0
                for m in range(len(points)-1):
                    n=m+1
                    while n<=len(points)-1:
                        dist=np.sqrt(np.square(points[n][1]-points[m][1])+np.square(points[n][0]-points[m][0]))
                        dist_list.append(dist)
                        dist_d[count] = [points[m], points[n]]
                        n=n+1
                        count=count+1
                dist_index=sorted(range(len(dist_list)), key=lambda k: dist_list[k])
                cc = 1
                for i in range(len(dist_index)):
                    if cc <= stop_at+1:
                        new_edg = dist_d[dist_index[i]]
                        flagp=0
                        for ij in range(len(points)):
                            if ((points[ij] == new_edg[0]) or (points[ij] ==  new_edg[1])):
                                ijk=1
                            else:
                                if (isBetween(new_edg[0],new_edg[1],points[ij])):
                                    flagp=1
                        if(flagp==0):
                            edg.append(dist_d[dist_index[i]])
                            cc = cc+1
    #print("edg",edg)
    edg.sort()
    edg = list(edg for edg,_ in itertools.groupby(edg))
    l_en=len(edg)
    print("E = {")
    for i in range(l_en):
        v1=edg[i][0]
        v2=edg[i][1]
        las=[]
        for k,v in V.items():
            if v1==v:
                las.append(k)
            if v2==v:
                las.append(k)
        print("<"+str(las[0])+","+str(las[1])+">", end="")
        if i != l_en-1:
            print(",")
        else:
            print("")
        #print(las)
    print("}")
    #return edg
            

def main():
    warnings.filterwarnings("ignore")
    # YOUR MAIN CODE GOES HERE

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    db = {}
    V = {}
    E = {}
    while True:
        line = sys.stdin.readline()
        if line == "":
            break
        #print("read a line:", line)
        try:
            msg,cmd,st,coor = parse_input(line)
            #print(msg, cmd, st, coor)
            if cmd == "add" or cmd == "mod":
                coor = proper_coordinates(coor)
                db = street_database(db, cmd, st, coor)
            elif cmd == "rm":
                db = street_database(db, cmd, st, coor)
                #print(db)
            elif cmd == "gg":
                db = street_database(db, cmd, st, coor)
                V, intersections = find_vertices(db, V)
                #print(db)
                #print(V)
                print("V = {")
                for key, value in sorted(V.items()):
                    val = tuple(value)
                    print(str(key)+': '+str(val))
                print("}")
                find_edges(db, intersections, V, E)
                #print(E)


        except Exception as e:
            print('Error: ' + str(e))
            print('Error: ' + str(e), file = sys.stderr)

    #print("Finished reading input")
    # return exit code 0 on successful termination
    sys.exit(0)

if __name__ == "__main__":
    main()
