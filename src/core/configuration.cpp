#include "configuration.hpp"


void configuration::destroy() {
	m_running = false;
}

void configuration::init(const file& save_file) {
	m_running = true;
	m_save_file = std::make_unique<file>(save_file.get_path());
	load();
	g_thread_pool->push([this] {
		while (m_running) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			attempt_save();
		}
	});
}

void configuration::attempt_save() {
	const nlohmann::json j = *this;

	if (deep_compare(m_options, j, true))
		save();
}

bool configuration::load() {
	m_default_options = *this;

	std::ifstream file(m_save_file->get_path());

	if (!m_save_file->exists()) {
		write_default_config();

		file.open(m_save_file->get_path());
	}

	try {
		file >> m_options;

		file.close();
	}
	catch (const std::exception&) {
		file.close();

		std::cout << "Detected corrupt settings, writing default config..." << std::endl;

		write_default_config();

		return load();
	}
	const bool should_save = deep_compare(m_options, m_default_options);
	try {
		from_json(m_options, *this);
	}
	catch (const nlohmann::detail::type_error& e) {
		std::cout << e.what() << std::endl;

		return false;
	}

	if (should_save) {
		std::cout << "Updating settings." << std::endl;
		save();
	}

	return true;
}

bool configuration::deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value)
{
	bool should_save = false;

	for (auto& e : default_settings.items())
	{
		const std::string& key = e.key();

		if (current_settings.count(key) == 0 || (compare_value && current_settings[key] != e.value()))
		{
			current_settings[key] = e.value();

			should_save = true;
		}
		else if (current_settings[key].is_object() && e.value().is_object())
		{
			if (deep_compare(current_settings[key], e.value(), compare_value))
				should_save = true;
		}
		else if (!current_settings[key].is_object() && e.value().is_object())
		{
			current_settings[key] = e.value();

			should_save = true;
		}
	}

	return should_save;
}

bool configuration::save()
{
	std::ofstream file(m_save_file->get_path(), std::ios::out | std::ios::trunc);
	nlohmann::json j = *this;
	file << j.dump(4);
	file.close();

	return true;
}

bool configuration::write_default_config()
{
	std::ofstream file(m_save_file->get_path(), std::ios::out | std::ios::trunc);
	nlohmann::json j = *this;
	file << j.dump(4);
	file.close();

	return true;
}