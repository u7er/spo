function getName($len){
    -join (1..$len | % { Get-Random -InputObject ("afgtdUHS45B-7634kLqO".ToCharArray())})
}

1..1535 | % -PipelineVariable NAME { getName(Get-Random -Minimum 5 -Maximum 17) } | % -PipelineVariable VALUE { getName(Get-Random -Minimum 12 -Maximum 100) } | % {New-Item -Name $NAME -Value $VALUE -ItemType "file"}

dir .\ -*;

Get-ChildItem .\ | where { $_.Name -match "\d"};
