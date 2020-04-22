s = input()
t = ""
u = ""

while s or t:
    if t and s and t[-1] <= min(s):
        u += t[-1]
        t = t[:-1]
    elif s:
        t += s[0]
        s = s[1:]
    else:
        u += t[-1]
        t = t[:-1]

print("".join(u))
