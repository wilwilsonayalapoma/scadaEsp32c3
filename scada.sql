-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 30-09-2025 a las 17:07:44
-- Versión del servidor: 10.4.32-MariaDB
-- Versión de PHP: 8.0.30

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `scada`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `ordenes`
--

CREATE TABLE `ordenes` (
  `id` int(11) NOT NULL,
  `nombre` varchar(50) DEFAULT NULL,
  `estado` varchar(10) DEFAULT NULL,
  `modo` varchar(10) DEFAULT NULL,
  `fecha` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `ordenes`
--

INSERT INTO `ordenes` (`id`, `nombre`, `estado`, `modo`, `fecha`) VALUES
(47, 'RELAY1', 'OFF', 'AUTO', '2025-09-30 14:53:58');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `reles`
--

CREATE TABLE `reles` (
  `id` int(11) NOT NULL,
  `nombre` varchar(50) DEFAULT NULL,
  `estado` varchar(10) DEFAULT NULL,
  `fecha` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `reles`
--

INSERT INTO `reles` (`id`, `nombre`, `estado`, `fecha`) VALUES
(1, 'Relay1', 'ON', '2025-09-30 13:48:06'),
(2, 'Relay1', 'ON', '2025-09-30 14:18:45'),
(3, 'Relay1', 'ON', '2025-09-30 14:41:27'),
(4, 'Relay1', 'OFF', '2025-09-30 14:41:32'),
(5, 'Relay1', 'ON', '2025-09-30 14:41:36'),
(6, 'Relay1', 'OFF', '2025-09-30 14:41:45'),
(7, 'Relay1', 'ON', '2025-09-30 14:42:48'),
(8, 'Relay1', 'OFF', '2025-09-30 14:42:52'),
(9, 'Relay1', 'ON', '2025-09-30 14:43:33'),
(10, 'Relay1', 'OFF', '2025-09-30 14:44:14'),
(11, 'Relay1', 'ON', '2025-09-30 14:46:45'),
(12, 'Relay1', 'OFF', '2025-09-30 14:46:48'),
(13, 'Relay1', 'ON', '2025-09-30 14:46:52'),
(14, 'Relay1', 'OFF', '2025-09-30 14:46:54'),
(15, 'Relay1', 'ON', '2025-09-30 14:47:59'),
(16, 'Relay1', 'OFF', '2025-09-30 14:48:03'),
(17, 'Relay1', 'ON', '2025-09-30 14:52:58'),
(18, 'Relay1', 'OFF', '2025-09-30 14:53:03');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `sensores`
--

CREATE TABLE `sensores` (
  `id` int(11) NOT NULL,
  `nombre` varchar(50) DEFAULT NULL,
  `valor` varchar(10) DEFAULT NULL,
  `fecha` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `sensores`
--

INSERT INTO `sensores` (`id`, `nombre`, `valor`, `fecha`) VALUES
(1139, 'SensorNivel', 'OFF', '2025-09-30 15:06:49'),
(1140, 'SensorNivel', 'OFF', '2025-09-30 15:06:51'),
(1141, 'SensorNivel', 'OFF', '2025-09-30 15:06:53'),
(1142, 'SensorNivel', 'OFF', '2025-09-30 15:06:55'),
(1143, 'SensorNivel', 'OFF', '2025-09-30 15:06:58'),
(1144, 'SensorNivel', 'OFF', '2025-09-30 15:07:00'),
(1145, 'SensorNivel', 'OFF', '2025-09-30 15:07:02'),
(1146, 'SensorNivel', 'OFF', '2025-09-30 15:07:04'),
(1147, 'SensorNivel', 'OFF', '2025-09-30 15:07:06');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `ordenes`
--
ALTER TABLE `ordenes`
  ADD PRIMARY KEY (`id`);

--
-- Indices de la tabla `reles`
--
ALTER TABLE `reles`
  ADD PRIMARY KEY (`id`);

--
-- Indices de la tabla `sensores`
--
ALTER TABLE `sensores`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `ordenes`
--
ALTER TABLE `ordenes`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=48;

--
-- AUTO_INCREMENT de la tabla `reles`
--
ALTER TABLE `reles`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=19;

--
-- AUTO_INCREMENT de la tabla `sensores`
--
ALTER TABLE `sensores`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1148;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
