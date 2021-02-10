module TestCall

using SpecialFunctions

Base.@ccallable erfv(x::Vector{Float64})::Vector{Float64} = SpecialFunctions.erf.(x)

Base.@ccallable function erfv2(out::Vector{Float64}, x::Vector{Float64})::Cvoid
    # @assert length(out) == length(x)
    @inbounds for i in eachindex(out)
        out[i] = SpecialFunctions.erf(x[i])
    end
end



export erfv, erfv2

end 