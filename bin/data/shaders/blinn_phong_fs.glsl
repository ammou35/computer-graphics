#version 330

in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;

out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

uniform int num_lights;
uniform vec3 light_positions[30];
uniform vec3 light_colors[30];
uniform vec3 light_directions[30]; // for spot and directional
uniform int light_type[30];        // 0 = point, 1 = directional, 2 = spotlight
uniform float spot_cutoffs[30];    // cos(angle) in radians for spotlight

uniform sampler2D tex0;

void main()
{
  vec3 n = normalize(surface_normal);
  vec3 v = normalize(-surface_position);
  vec3 tex_color = texture(tex0, surface_texcoord).rgb;

  vec3 result = color_ambient;

  for (int i = 0; i < 30; ++i) {
    if (i >= num_lights) break;

    vec3 l;
    float attenuation = 1.0;
    bool skip = false;

    if (light_type[i] == 0) {
      // Point light
      l = normalize(light_positions[i] - surface_position);
    }
    else if (light_type[i] == 1) {
      // Directional light (direction already in view space)
      l = normalize(-light_directions[i]); // from light to surface
    }
    else if (light_type[i] == 2) {
      // Spotlight
      vec3 light_to_frag = surface_position - light_positions[i];
      float distance = length(light_to_frag);
      l = normalize(-light_to_frag);

      float spot_cos = dot(l, normalize(-light_directions[i]));
      if (spot_cos < spot_cutoffs[i]) {
    	skip = true; // outside cone
      } else {
    	float spot_fade = (spot_cos - spot_cutoffs[i]) / (1.0 - spot_cutoffs[i]);
    	attenuation = pow(spot_fade, 4.0); // soft falloff
      }
    }

    if (!skip) {
      vec3 h = normalize(v + l);
      float diff = max(dot(n, l), 0.0);
      float spec = pow(max(dot(n, h), 0.001), brightness);

      vec3 diffuse = diff * color_diffuse * light_colors[i] * attenuation;
      vec3 specular = spec * color_specular * light_colors[i] * attenuation;

      result += diffuse + specular;
    }
  }

  vec3 final_color = tex_color * result;
  fragment_color = vec4(final_color, 1.0);
}
