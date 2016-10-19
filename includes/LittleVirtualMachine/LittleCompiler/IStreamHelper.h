#ifndef LITTLEVIRTUALMACHINE_ISTREAMHELPER_H
#define LITTLEVIRTUALMACHINE_ISTREAMHELPER_H

#include <string>

namespace lvm {
	namespace compiler {
		class IStreamHelper {
		public:
			virtual ~IStreamHelper() {};

			virtual void open(std::string& path) = 0;

			virtual std::istream& get_ro_stream() = 0;

			virtual std::ostream& get_rw_stream() = 0;

		protected:
			IStreamHelper() {}
		};
	}
}

#endif //LITTLEVIRTUALMACHINE_ISTREAMHELPER_H
