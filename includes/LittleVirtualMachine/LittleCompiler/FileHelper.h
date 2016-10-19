#ifndef LITTLEVIRTUALMACHINE_FILEHELPER_H
#define LITTLEVIRTUALMACHINE_FILEHELPER_H

#include <string>
#include <fstream>
#include "LittleVirtualMachine/LittleCompiler/IStreamHelper.h"

namespace lvm {
	namespace compiler {
		class FileHelper : public IStreamHelper {
		public:
			FileHelper() {}

			virtual ~FileHelper() {
				if (stream_.is_open())
					stream_.close();
			};

			virtual void open(std::string& path) {
				if (stream_.is_open())
					stream_.close();

				stream_.open(path);
			};

			virtual std::istream& get_ro_stream() { return stream_; }

			virtual std::ostream& get_rw_stream() { return stream_; }

		private:
			std::fstream stream_;
		};
	}
}

#endif //LITTLEVIRTUALMACHINE_FILEHELPER_H
