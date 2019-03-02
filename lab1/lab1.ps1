# 1-2
function getName($len){
    -join (1..$len | % { Get-Random -InputObject ("afgtdUHS45B-7634kLqO".ToCharArray())})
}

function getSizeFile($len){
    -join (1..$len | % { [char[]](0..127) -match ‘[0-9]’ | GetRandom })
}

1..1535 | % -PipelineVariable NAME { getName(Get-Random -Minimum 5 -Maximum 17) } | % -PipelineVariable VALUE { getSizeFile(Get-Random -Minimum 100 -Maximum 1024) } | % {New-Item -Name $NAME -Value $VALUE -ItemType "file"} | Out-Null;
# 3
dir .\ -*;

# 4
$nr = 0;
Get-ChildItem .\ | Sort-Object -Property lenght | % { Rename-Item $_ -NewName (("{0}" + " " + $_.ToString()) -f $nr++) };

