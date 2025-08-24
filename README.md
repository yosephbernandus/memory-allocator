# A Learning stuff about allocted memory heap and stack

```
malloc testing 
malloc(16) -> 0x557af71e7430
head = 0x557af71e7000, tail = 0x557af71e7418 
address = 0x557af71e7000, size = 1024, is_free = 0, next = 0x557af71e7418
address = 0x557af71e7418, size = 16, is_free = 0, next = (nil)

malloc(32) -> 0x557af71e7458
head = 0x557af71e7000, tail = 0x557af71e7440 
address = 0x557af71e7000, size = 1024, is_free = 0, next = 0x557af71e7418
address = 0x557af71e7418, size = 16, is_free = 0, next = 0x557af71e7440
address = 0x557af71e7440, size = 32, is_free = 0, next = (nil)

head = 0x557af71e7000, tail = 0x557af71e7440 
address = 0x557af71e7000, size = 1024, is_free = 0, next = 0x557af71e7418
address = 0x557af71e7418, size = 16, is_free = 0, next = 0x557af71e7440
address = 0x557af71e7440, size = 32, is_free = 0, next = (nil)

calloc(4, int) -> 0x557af71e7468
head = 0x557af71e7000, tail = 0x557af71e7450 
address = 0x557af71e7000, size = 1024, is_free = 0, next = 0x557af71e7418
address = 0x557af71e7418, size = 16, is_free = 0, next = 0x557af71e7440
address = 0x557af71e7440, size = 32, is_free = 0, next = 0x557af71e7450
address = 0x557af71e7450, size = 93986620339280, is_free = 0, next = (nil)

realloc(third_pointer, 64) -> 0x557af71e7468
head = 0x557af71e7000, tail = 0x557af71e7450 
address = 0x557af71e7000, size = 1024, is_free = 0, next = 0x557af71e7418
address = 0x557af71e7418, size = 16, is_free = 0, next = 0x557af71e7440
address = 0x557af71e7440, size = 32, is_free = 0, next = 0x557af71e7450
address = 0x557af71e7450, size = 93986620339280, is_free = 0, next = (nil)

head = 0x557af71e7000, tail = 0x557af71e7450 
address = 0x557af71e7000, size = 1024, is_free = 0, next = 0x557af71e7418
address = 0x557af71e7418, size = 16, is_free = 0, next = 0x557af71e7440
address = 0x557af71e7440, size = 32, is_free = 0, next = 0x557af71e7450
address = 0x557af71e7450, size = 93986620339280, is_free = 0, next = (nil)
```
