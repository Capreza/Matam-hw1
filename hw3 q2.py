
def create_letter_list(input_string: str):
    letter_list = []
    for letter in input_string:
        curr_letter = letter.lower()
        letter_list.append(curr_letter)
    return letter_list

def check_strings(s1: str, s2: str):
    letter_list1 = create_letter_list(s1)
    letter_list2 = create_letter_list(s2)
    for letter in letter_list1:
        if letter_list2.count(letter) == 0:
            return False
        else:
            letter_list2.remove(letter)
    return True
