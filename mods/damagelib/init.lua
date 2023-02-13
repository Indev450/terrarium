damagelib = {}

dofile("equipment.lua")

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

    victim:hurt(amount, damage_source or damager, damage.knockback, damage.invincible_time)

    return amount
end
