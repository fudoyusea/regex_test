import rstr
from xeger import Xeger


# generate string based input charset.
gene_str1 = rstr.rstr('ABC')
print(gene_str1)

gene_str2 = rstr.rstr('ABC', 4)
print(gene_str2)

gene_str3 = rstr.rstr('ABC', 4, 10)
print(gene_str3)

gene_str4 = rstr.rstr('ABC', include='@')
print(gene_str4)

gene_str5 = rstr.rstr('ABC', exclude='B')
print(gene_str5)

# generate strings based regex.
# random number
gene_str6 = rstr.xeger(r'^([1-9]([0-9]{4,10}))')
print(gene_str6)

# domain name
gene_str8 = Xeger(limit=30)
gene_str8 = gene_str8.xeger("^https?://[\w-]+(\.[\w-]+){1,2}(/[\w-]{3,6}){0,2}(\?[\w_]{4,6}=[\w_]{4,6}(&[\w_]{4,6}=[\w_]{4,6}){0,2})?")
print(gene_str8)

# email
gene_str9 = Xeger(limit=20)
gene_str9 = gene_str9.xeger("\w{6,12}@[a-z0-9]{3}\.(com|cn|edu\.cn)")
print(gene_str9)

# phone number
gene_str10 = Xeger(limit=11)
gene_str10 = gene_str10.xeger("1(3|5|7|8|9)\d{9}")
print(gene_str10)

# float number
gene_str11 = Xeger(limit=20)
gene_str11 = gene_str11.xeger("-?[1-9]\d*\.\d+")
print(gene_str11)
