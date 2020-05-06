from heapq import heappush, heapify

w, h, n = map(int, input().split())

H_cuts = [0, h]
V_cuts = [0, w]
# heapify(H_cuts)
# heapify(V_cuts)
last_big_H = h
last_big_V = w
for i in range(n):
    direction, dist = input().split()
    distance = int(dist)
    if direction == "H":
        # heappush(H_cuts, distance)
        # H_cuts.append(distance)
        # H_cuts.sort(reverse=True)
        inserted = False
        big_H = 0
        j = 0
        while j < len(H_cuts)-1:
            if not inserted and H_cuts[j+1] > distance and distance > H_cuts[j]:
                H_cuts.insert(j+1, distance)
                inserted = True
            diff = H_cuts[j+1] - H_cuts[j]
            if diff > big_H:
                big_H = diff
                # if big_H == last_big_H:
                #     break
            j += 1
        last_big_H = big_H
    elif direction == "V":
        # heappush(V_cuts, distance)
        # V_cuts.append(distance)
        # V_cuts.sort(reverse=True)
        inserted = False
        big_V = 0
        j = 0
        while j < len(V_cuts)-1:
            if not inserted and V_cuts[j+1] > distance and distance > V_cuts[j]:
                V_cuts.insert(j+1, distance)
                inserted = True
            diff = V_cuts[j+1] - V_cuts[j]
            if diff > big_V:
                big_V = diff
                # if big_V == last_big_V:
                #     break
            j += 1
        last_big_V = big_V

    print(last_big_H*last_big_V)

