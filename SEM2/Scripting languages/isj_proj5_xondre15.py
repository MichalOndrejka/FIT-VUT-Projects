def gen_quiz(qpool, *args, **kwargs):
    if 'altcodes' in kwargs.keys():
        altcodes = kwargs['altcodes']
    else:
        altcodes = 'ABCDEF'
    
    if 'quiz' in kwargs.keys():
        quiz = kwargs['quiz']
    else:
        quiz = []

    for index in args:
        try:
            item = qpool[index]
        except IndexError:
            print(f'Ignoring index {index} - list index out of range')
            continue
        
        answers = item[1][:]
        answers_len = len(answers)
        for answer_index in range(answers_len):
            if answer_index < len(altcodes):
                answers[answer_index] = altcodes[answer_index] + ': ' + answers[answer_index]
            else:
                answers.remove(item[1][answer_index])
        item = list(item)
        item[1] = answers
        item = tuple(item)
        quiz.append(item)
    return quiz