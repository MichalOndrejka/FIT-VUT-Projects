def first_with_given_key(iterable, key=repr):
    func = lambda item: key(item)
    keys = set()
    for item in iterable:
        item_key = func(item)
        if item_key not in keys:
            keys.add(item_key)
            yield item
            