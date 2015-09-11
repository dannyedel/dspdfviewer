#pragma once

#include <QString>

enum class WindowRole {
	AudienceWindow,
	PresenterWindow
};

// Return "Audience_Window" or "Secondary_Window"
QString to_QString ( const WindowRole& );
