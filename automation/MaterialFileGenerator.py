MTLlist = []

def parseMTL(Sinfo):
    import re
    floatlex = re.compile(r"^([-+]?\d*\.\d+|\d+)")
    
    Sinfo = Sinfo.strip().split('\n')
    
    name = Sinfo.pop(0)
    values = {
    "ambient":[float(floatlex.search(Sinfo.pop(0)).group()) for _ in range(4)],
    "diffuse":[float(floatlex.search(Sinfo.pop(0)).group()) for _ in range(4)],
    "specular":[float(floatlex.search(Sinfo.pop(0)).group()) for _ in range(4)],
    "shininess":[float(floatlex.search(Sinfo.pop(0)).group())],
    }

    MTLlist.append((name, values))

def generateMaterialFile():
    def toinitializerlistfloat(valuelist):
        floatliteral = map(lambda x : f"{x}f", valuelist)
        floatliterallist = ', '.join(floatliteral)
        return f"{{{floatliterallist}}}"

    for name, values in MTLlist:
        initstatement = []
        initstatement.append(f"Material {name}(")
        
        initarrays = []
        # No Emission for materials
        initarrays.append("{0.0f, 0.0f, 0.0f, 0.0f}")
        for value in values.values():
            initarrays.append(f"{toinitializerlistfloat(value)}")
        
        initstatement.append(",\n".join(initarrays))
        
        initstatement.append(");")
        
        print('\n'.join(initstatement))
        print()

parseMTL("""Brass
0.329412
0.223529
0.027451
1.0
0.780392
0.568627
0.113725
1.0
0.992157
0.941176
0.807843
1.0
27.89743616""")

parseMTL("""Bronze
0.2125
0.1275
0.054
1.0
0.714
0.4284
0.18144
1.0
0.393548
0.271906
0.166721
1.0
25.6""")

parseMTL("""Polished_bronze
0.25
0.148
0.06475
1.0
0.4
0.2368
0.1036
1.0
0.774597
0.458561
0.200621
1.0
76.8""")

parseMTL("""Chrome
0.25
0.25
0.25
1.0
0.4
0.4
0.4
1.0
0.774597
0.774597
0.774597
1.0
76.8""")

parseMTL("""Copper
0.19125
0.0735
0.0225
1.0
0.7038
0.27048
0.0828
1.0
0.256777
0.137622
0.086014
1.0
12.8""")

parseMTL("""Polished_copper
0.2295
0.08825
0.0275
1.0
0.5508
0.2118
0.066
1.0
0.580594
0.223257
0.0695701
1.0
51.2""")

parseMTL("""Emerald
0.0215
0.1745
0.0215
0.55 (or 1.0)
0.07568
0.61424
0.07568
0.55 (or 1.0)
0.633
0.727811
0.633
0.55 (or1.0)
76.8
""")

parseMTL("""Gold
0.24725
0.1995
0.0745
1.0
0.75164
0.60648
0.22648
1.0
0.628281
0.555802
0.366065
1.0
51.2
""")

parseMTL("""Polished_gold
0.24725
0.2245
0.0645
1.0
0.34615
0.3143
0.0903
1.0
0.797357
0.723991
0.208006
1.0
83.2
""")

parseMTL("""Jade
0.135
0.2225
0.1575
0.95 (or 1.0)
0.54
0.89
0.63
0.95 (or 1.0)
0.316228
0.316228
0.316228
0.95 (or 1.0)
12.8
""")

parseMTL("""Obsidian
0.05375
0.05
0.06625
0.82 (or 1.0)
0.18275
0.17
0.22525
0.82 (or 1.0)
0.332741
0.328634
0.346435
0.82 (or 1.0)
38.4
""")

parseMTL("""Pearl
0.25
0.20725
0.20725
0.922 (or 1.0)
1.0
0.829
0.829
0.922 (or 1.0)
0.296648
0.296648
0.296648
0.922 (or 1.0)
11.264
""")

parseMTL("""Pewter
0.105882
0.058824
0.113725
1.0
0.427451
0.470588
0.541176
1.0
0.333333
0.333333
0.521569
1.0
9.84615
""")

parseMTL("""Ruby
0.1745
0.01175
0.01175
0.55 (or 1.0)
0.61424
0.04136
0.04136
0.55 (or 1.0)
0.727811
0.626959
0.626959
0.55 (or 1.0)
76.8
""")

parseMTL("""Silver
0.19225
0.19225
0.19225
1.0
0.50754
0.50754
0.50754
1.0
0.508273
0.508273
0.508273
1.0
51.2
""")

parseMTL("""Polished_silver
0.23125
0.23125
0.23125
1.0
0.2775
0.2775
0.2775
1.0
0.773911
0.773911
0.773911
1.0
89.6
""")

parseMTL("""Turquoise
0.1
0.18725
0.01745
0.8 (or 1.0)
0.396
0.74151
0.69102
0.8 (or 1.0)
0.297254
0.30829
0.306678
0.8 (or 1.0)
12.8
""")

parseMTL("""Plastic_black
0.0
0.0
0.0
1.0
0.01
0.01
0.01
1.0
0.5
0.5
0.5
1.0
32
""")

parseMTL("""Plastic_cyan
0.0
0.1
0.06
1.0
0.0
0.50980392
0.50980392
1.0
0.50196078
0.50196078
0.50196078
1.0
32
""")

parseMTL("""Plastic_green
0.0
0.0
0.0
1.0
0.1
0.35
0.1
1.0
0.45
0.55
0.45
1.0
32
""")

parseMTL("""Plastic_red
0.0
0.0
0.0
1.0
0.5
0.0
0.0
1.0
0.7
0.6
0.6
1.0
32
""")

parseMTL("""Plastic_white
0.0
0.0
0.0
1.0
0.55
0.55
0.55
1.0
0.7
0.7
0.7
1.0
32
""")

parseMTL("""Plastic_yellow
0.0
0.0
0.0
1.0
0.5
0.5
0.0
1.0
0.6
0.6
0.5
1.0
32
""")

parseMTL("""Rubber_black
0.02
0.02
0.02
1.0
0.01
0.01
0.01
1.0
0.4
0.4
0.4
1.0
10
""")

parseMTL("""Rubber_cyan
0.0
0.05
0.05
1.0
0.4
0.5
0.5
1.0
0.04
0.7
0.7
1.0
10
""")

parseMTL("""Rubber_green
0.0
0.05
0.0
1.0
0.4
0.5
0.4
1.0
0.04
0.7
0.04
1.0
10
""")

parseMTL("""Rubber_red
0.05
0.0
0.0
1.0
0.5
0.4
0.4
1.0
0.7
0.04
0.04
1.0
10
""")

parseMTL("""Rubber_white
0.05
0.05
0.05
1.0
0.5
0.5
0.5
1.0
0.7
0.7
0.7
1.0
10
""")

parseMTL("""Rubber_yellow
0.05
0.05
0.0
1.0
0.5
0.5
0.4
1.0
0.7
0.7
0.04
1.0
10
""")

generateMaterialFile()
