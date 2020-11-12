#!/usr/bin/env python3

IOList = [
    {
        "ActionFunction": "static void Pang_KeyboardAction(unsigned char key, int x, int y)",
        "IO": {
            "keySPACE": "' '",
        }
    },
    {
        "ActionFunction": "static void Pang_SpecialKeyboardAction(int key, int x, int y)",
        "IO": {
            "keyLEFT": "GLUT_KEY_LEFT",
            "keyUP": "GLUT_KEY_UP",
            "keyRIGHT": "GLUT_KEY_RIGHT",
            "keyDOWN": "GLUT_KEY_DOWN",
        }
    },
]

print("static PangIO PIO;")
print()

for IOinfo in IOList:
    print(f"{IOinfo['ActionFunction']} {{")
    print("\tswitch (key) {")

    for key, keyvalue in IOinfo["IO"].items():
        print(f"\tcase {keyvalue}:")
        print(f"\t\tPIO.set{key}();")

        print("\t\tbreak;")
    
    print("\t}")
    print()
    print("\tglutPostRedisplay();")
    print("}")

## class PangIO definition
classvar = []
for IOinfo in IOList:
    for key, keyvalue in IOinfo["IO"].items():
        classvar.append(key)

print("class PangIO {")
print("private:")

for key in classvar:
    print(f"\tbool {key};")

print()
print("public:")
print("\tPangIO() : " + ", ".join(map(lambda key : f"{key}(false)", classvar)) + " {}")

print()
for key in classvar:
    print(f"\tvoid set{key}();")
print()

print("\tvoid submit();")
print("};")
print()

## class PangIO functions

for key in classvar:
    print(f"void PangIO::set{key}() {{")
    print(f"\t{key} = true;")
    print("}")
    print()

print("void PangIO::submit() {")

for key in classvar:
    print(f"\tif({key}) {{")
    print(f"\t\t{key} = false;")
    print("\t\t")
    print("\t}")

print("}")
