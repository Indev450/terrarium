damagelib = {
    damage_color = {},
}

dofile("equipment.lua")
dofile("entity.lua")

function damagelib.calc_damage(victim_gear, damage, damager_gear)
    local amount = damage.amount
    local element = damage.element or "normal"

    local resistance = damagelib.equipment.get_elemental_resistance(victim_gear, element)
    local protection = damagelib.equipment.get_protection(victim_gear)
    local power = damagelib.equipment.get_elemental_power(damager_gear, element)
    local modifier = damagelib.equipment.get_damage_modifier(damager_gear)

    amount = math.max(1, amount * resistance * power * modifier - protection)

    return amount
end

function damagelib.hurt(victim, damage, damager, damage_source)
    local amount = damagelib.calc_damage(victim.equipment, damage, damager and damager.equipment)

    local pos = victim.ref:get_position()
    pos.x = victim.ref:get_center().x + (math.random()-0.5) * 2
    pos.y = pos.y + (math.random()-0.5) * 2

    if victim:hurt(amount, damage_source or damager, damage.knockback, damage.invincible_time) then
        terrarium.new_entity("damagelib:damage_indicator", pos, { amount = amount, color = damage.color or damagelib.damage_color[damage.element or "normal"] })
    end

    return amount
end
