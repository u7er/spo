# 1-2
function getName($len){
    -join (1..$len | % { Get-Random -InputObject ("afgtdUHS45B-7634kLqO".ToCharArray())})
}

1..1535 | % -PipelineVariable NAME { getName(Get-Random -Minimum 5 -Maximum 17) } | % -PipelineVariable VALUE { getName(Get-Random -Minimum 12 -Maximum 100) } | % {New-Item -Name $NAME -Value $VALUE -ItemType "file"} | Out-Null;
# 3
dir .\ -*;

# 4
$nr = 0;
Get-ChildItem .\ | Sort-Object -Property lenght | % { Rename-Item $_ -NewName (("{0}" + " " + $_.ToString()) -f $nr++) };

