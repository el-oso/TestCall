using TestCall
erfv(rand(3))
a = Vector{Float64}(undef, 3)
erfv2(a, rand(3))