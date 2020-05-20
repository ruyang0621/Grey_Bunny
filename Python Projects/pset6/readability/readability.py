from cs50 import get_string


def main():
    text = get_string("Text: ")
    
    letter_count = 0
    word_count = 1
    sentence_count = 0
    
    for n in range(len(text)):
        if text[n].isalpha():
            letter_count += 1
        
        if text[n].isspace() and (text[n + 1].isalpha or text[n + 1].ispunct):
            word_count += 1
        
        if ord(text[n]) in [33, 46, 63]:
            sentence_count += 1
            
    i = Index(letter_count, word_count, sentence_count)
    
    if i >= 16:
        print("Grade 16+")
    elif i < 1:
        print("Before Grade 1")
    else:
        print("Grade", i)


def Index(letters, words, sentences):
    l = letters / words * 100
    s = sentences / words * 100
    return round(0.0588 * l - 0.296 * s - 15.8)
    
    
main()  
