# Door names cannot have spaces but can have any alphanumeric character.
# Please add a the doors you want to manage to the following list, eg
# 1 door:
#   doors = ["doorname"]
#2 or more doors:
#   doors = ["doorname1", "doorname2"]

doors = ["topgarage", "frontdoor", "bottomgarage"]
#make sure arduino sending json identifier that maps to the above

#leave the rest alone
#createString = ""
#insertString = ""
#for i in doors:
    #createString +=(i +' INTEGER, ')
    ## gives ["topgarage INTEGER, frontdoor INTEGER, bottomgarage INTEGER"]
    #insertString +=(' ?,')
    
#print createString
#print insertString
