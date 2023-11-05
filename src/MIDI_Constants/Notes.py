notes = ["C", "C♯/D♭", "D", "D♯/E♭", "E", "F", "F♯/G♭", "G", "G♯/A♭", "A", "A♯/B♭", "B"]
print("|Octave| C |C♯/D♭| D |D♯/E♭| E | F  |F♯/G♭| G |G♯/A♭| A |A♯/B♭| B  |")
print(f"|:-----|{'---:|' * 12}")
for octave in range(-1, 9):
    notes = [f"{base + 12 * (octave + 1):4}" for base in range(12)]
    print(f"|{octave:<6}|{'|'.join(notes)}|")
