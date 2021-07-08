# Strings-Encrypter
C++ WinAPI program which encrypts strings in `.data` section of Windows PE with RC4


[![forthebadge](https://forthebadge.com/images/badges/made-with-c.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/it-works-why.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/gluten-free.svg)](https://forthebadge.com)

- Strings are naively defined to be a printable sequence of characters in the `.data` section of a `PE` that ends in a null byte.
- References to found strings are stored in a simple linked list data structure.
- RC4 key blob is exported to a file so that it can be imported by a separate decryptor tool.
- Code performs _zero_ error-handling. **It is neither meant to be robust nor complete** as it is just a proof-of-concept.
