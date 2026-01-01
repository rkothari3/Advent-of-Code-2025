with open('input.txt', 'r') as f:
    lines = f.readlines()

count = 0
for line in lines:
    line = line.strip()
    if line and ':' in line and '#' not in line and '.' not in line:
        parts = line.split(':')
        if parts[1]:
            size = parts[0].split('x')
            width = int(size[0])
            height = int(size[1])
            
            presents = parts[1].split()
            total_presents = 0
            for p in presents:
                total_presents = total_presents + int(p)
            
            area = width * height
            if 9 * total_presents <= area:
                count = count + 1

print(count)