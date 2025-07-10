* **Commands With env var**
`[bash] asmaa@asmaa:~ master$` export var='echo hi asmaa | grep asmaa'
`[bash] asmaa@asmaa:~ master$` $var
    hi asmaa | grep asmaa
`[bash] asmaa@asmaa:~ master$` export var='cat hi.txt'
`[bash] asmaa@asmaa:~ master$` 
`[bash] asmaa@asmaa:~ master$` $var
    hi lsdjldsj fsdjfsj
`[bash] asmaa@asmaa:~ master$` export var='hi'
`[bash] asmaa@asmaa:~ master$` $var
    Command 'hi' not found, but can be installed with:
    sudo snap install hi
`[bash] asmaa@asmaa:~ master$` echo $var
    hi
`[bash] asmaa@asmaa:~ master$` export var='hi.txt'
`[bash] asmaa@asmaa:~ master$` cat $var
    hi lsdjldsj fsdjfsj
`[bash] asmaa@asmaa:~ master$`



