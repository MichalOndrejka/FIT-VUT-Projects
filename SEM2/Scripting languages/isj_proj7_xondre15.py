##!/usr/bin/env python3

def log_and_count(**count_kwargs):
    def decorator(function):
        def inner(*args, **kwargs):
            counter = count_kwargs["counts"]
            if "key" in count_kwargs:
                counter[count_kwargs["key"]] += 1
            else:
                counter[function.__name__] += 1
            print(f"called {function.__name__} with {args} and {kwargs}")
            return function(*args, **kwargs)
        return inner
    return decorator
