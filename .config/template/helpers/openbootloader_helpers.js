/**
 * Return if SW component has been enabled in the project (or at least its dependencies)
 * @param {object} used_components used components returned by SWProjectAPI.getUsedComponents getter
 * @param {string} component component to be check
 */
function helper_openbootloader_class_is_selected(used_components, component) {
  let result = 0;
  try {
    used_components.forEach((comp) => {
      if (
        comp["cgroup"] === "STM32CubeMX2 Codegen" &&
        comp["csub"].toUpperCase() === component.toUpperCase()
      ) {
        result = 1;
        return;
      }
    });
  } catch (e) {
    console.error(`[ERROR] openbootloader_helpers: ${e}`);
  }
  return result;
}

/**
 * Return if SW component has been enabled in the project (or at least its dependencies)
 * @param {object} used_components used components returned by SWProjectAPI.getUsedComponents getter
 * @param {string} component component to be check
 */
function helper_openbootloader_core_is_enabled(used_components, component) {
  let result = 0;
  try {
    used_components.forEach((comp) => {
      if (
        comp["cgroup"] === "openbootloader Core" &&
        comp["csub"].toUpperCase() === component.toUpperCase()
      ) {
        result = 1;
        return;
      }
    });
  } catch (e) {
    console.error(`[ERROR] openbootloader_helpers: ${e}`);
  }
  return result;
}

/**
 * Checks if the current item selection is valid.
 * @param {object} item - The host_instance item.
 * @returns {boolean}
 */
function helper_openbootloader_is_usart_instance(item) {
  const selection = item?.select_the_device_instance?.selection;
  return (typeof selection === 'string' && selection.includes('USART'));
}

/**
 * Return ip_name in a string format.
 * @param {string} value
 * @returns {string}
 */
function helper_openbootloader_get_ip_name(value) {
  if (typeof value !== 'string') {
    return '';
  }
  return value.replace(/\./g, '_');
}

module.exports = {
  helper_openbootloader_class_is_selected,
  helper_openbootloader_core_is_enabled,
  helper_openbootloader_is_usart_instance,
  helper_openbootloader_get_ip_name,
};
